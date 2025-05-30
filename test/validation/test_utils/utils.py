from pathlib import Path
from typing import Tuple, List, Any, Dict
import subprocess

from netCDF4 import Group, Dataset, Variable



def run_executable(executable: Path, *args: str) -> None:
    result = subprocess.run([str(executable), *args], capture_output=True, text=True)
    if result.returncode != 0:
        raise RuntimeError(f"{executable.name} failed:\n{result.stderr}")

def clean_directory(dir_path: Path):
    if dir_path.exists():
        for file in dir_path.glob("*"):
            file.unlink()
        dir_path.rmdir()

def collect_file_pairs(ref_dir: Path, test_dir: Path, ext: str) -> List[Tuple[Path, Path]]:
    pairs = [
        (ref, test)
        for test in sorted(test_dir.glob(f"*{ext}"))
        if (ref := ref_dir / test.name).exists()
    ]
    if not pairs:
        raise ValueError(f"No matching {ext} file pairs found.")
    return pairs

def parametrize_file_pairs(metafunc: Any, fixture_name: str, ref_dir: Path, out_dir: Path, ext: str):
    if fixture_name in metafunc.fixturenames:
        file_pairs = collect_file_pairs(ref_dir, out_dir, ext)
        metafunc.parametrize(fixture_name, file_pairs, ids=[p[1].name for p in file_pairs])


def extract_structure(file_path: Path) -> Dict[str, Dict[str, Dict[str, Any]]]:
    def recurse(group: Group) -> Dict[str, Dict[str, Dict[str, Any]]]:
        structure = {group.path: {name: describe_variable(var) for name, var in group.variables.items()}}
        for sub in group.groups.values():
            structure.update(recurse(sub))
        return structure
    with Dataset(file_path.as_posix(), "r") as ds:
        return recurse(ds)


def describe_variable(var: Variable) -> Dict[str, Any]:
    return {
        "dtype": var.dtype,
        "dimensions": var.dimensions,
        "attributes": set(var.ncattrs()),
        "data": var[...],
    }


def format_netcdf_assert_msg(file: str, group: str, var: str,
                             detail: str, ref_val: Any, test_val: Any) -> str:
    return (
        f"[NetCDF Mismatch] File: '{file}' | Group: '{group}' | Variable: '{var}'\n"
        f"Reason: {detail}\nExpected: {ref_val}\nActual:   {test_val}"
    )

def get_pytest_markers(metafunc: Any) -> List[str]:
    args = metafunc.config.invocation_params.args
    if "-m" in args:
        return [m.strip() for m in args[args.index("-m") + 1].split("or") if m.strip()]
    return metafunc.config.markers
