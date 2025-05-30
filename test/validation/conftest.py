import subprocess
import tarfile
import requests
import pytest
import numpy as np
from pathlib import Path
from netCDF4 import Dataset, Variable, Group
from typing import List, Tuple, Dict, Any
from FilePathConfig import OBS2IODA_V3_EXECUTABLE_PATH

def obs2ioda_test_directory() -> Path:
    return Path.cwd() / "test/validation"

def format_netcdf_assert_msg(file: str, group: str, variable: str, detail: str,
                             ref_val: Any, test_val: Any) -> str:
    return (
        f"[NetCDF Mismatch] File: '{file}' | Group: '{group}' | Variable: '{variable}'\n"
        f"Reason: {detail}\n"
        f"Expected: {ref_val}\n"
        f"Actual:      {test_val}"
    )

def describe_variable(var: Variable) -> Dict[str, Any]:
    return {
        "dtype": var.dtype,
        "dimensions": var.dimensions,
        "attributes": set(var.ncattrs()),
        "data": var[...]
    }

def extract_structure(nc_path: Path) -> Dict[str, Dict[str, Dict[str, Any]]]:
    def recurse(group: Group) -> Dict[str, Dict[str, Dict[str, Any]]]:
        structure: Dict[str, Dict[str, Dict[str, Any]]] = {group.path: {}}
        for name, var in group.variables.items():
            structure[group.path][name] = describe_variable(var)
        for subgroup in group.groups.values():
            structure.update(recurse(subgroup))
        return structure
    with Dataset(nc_path.as_posix(), "r") as ds:
        return recurse(ds)

def collect_file_pairs() -> List[Tuple[Path, Path]]:
    ref_dir = obs2ioda_test_directory() / "data/v3/reference"
    test_dir = obs2ioda_test_directory() / "data/v3/output"
    if not ref_dir.exists() or not test_dir.exists():
        raise FileNotFoundError("Reference or test directory does not exist.")
    file_pairs: List[Tuple[Path, Path]] = []
    for test_file in sorted(test_dir.glob("*.h5")):
        ref_file = ref_dir / test_file.name
        if not ref_file.exists():
            raise FileNotFoundError(f"Missing reference file for: {test_file.name}")
        file_pairs.append((ref_file, test_file))
    if not file_pairs:
        raise ValueError("No .h5 files found in test directory.")
    return file_pairs

def pytest_generate_tests(metafunc: Any) -> None:
    url = "https://www2.mmm.ucar.edu/obs2ioda/test_data/data.tar.gz"
    dest_dir = obs2ioda_test_directory()
    archive_path = dest_dir / Path(url).name
    if not archive_path.exists():
        dest_dir.mkdir(parents=True, exist_ok=True)
        with requests.get(url, stream=True) as r:
            r.raise_for_status()
            with open(archive_path, "wb") as f:
                for chunk in r.iter_content(chunk_size=8192):
                    f.write(chunk)
    extracted_dir = archive_path.with_suffix("").with_suffix("")
    if not extracted_dir.exists():
        with tarfile.open(archive_path, "r:gz") as tar:
            tar.extractall(path=dest_dir)
    if "-m" in metafunc.config.invocation_params.args:
        if not "validation" in metafunc.config.invocation_params.args:
            return
    input_dir = extracted_dir / "bufr"
    output_dir = extracted_dir / "v3/output"
    executable = Path(OBS2IODA_V3_EXECUTABLE_PATH)
    if not executable.exists():
        raise FileNotFoundError(f"obs2ioda_v3 executable not found at {executable}")
    if not output_dir.exists():
        output_dir.mkdir(exist_ok=True)
        for file in output_dir.glob("*"):
            file.unlink()
        for bufr_file in input_dir.glob("*gdas*"):
            cmd = [str(executable), "-i", str(input_dir), "-o", str(output_dir), bufr_file.name]
            try:
                subprocess.run(cmd, check=True, capture_output=True)
            except subprocess.CalledProcessError as e:
                raise RuntimeError(f"obs2ioda_v3 failed for {bufr_file.name}: {e.stderr}")
    if "file_pair" in metafunc.fixturenames:
        file_pairs = collect_file_pairs()
        metafunc.parametrize("file_pair", file_pairs, ids=[pair[1].name for pair in file_pairs])

@pytest.fixture(scope="session", autouse=True)
def teardown() -> None:
    yield
    output_dir = obs2ioda_test_directory() / "data/v3/output"
    if output_dir.exists():
        for file in output_dir.glob("*"):
            file.unlink()
        if output_dir.exists():
            output_dir.rmdir()

@pytest.fixture(scope="module")
def file_pair(request: Any) -> Tuple[Path, Path]:
    return request.param
