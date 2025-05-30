import requests
from pathlib import Path
import tarfile
from .base_suite import TestSuite
from .utils import run_executable
from .FilePathConfig import OBS2IODA_V3_EXECUTABLE_PATH


def setup_validation(output_dir: Path):
    url = "https://www2.mmm.ucar.edu/obs2ioda/test_data/data.tar.gz"
    base_dir = Path.cwd() / "test/validation"
    archive = base_dir / Path(url).name
    extracted = archive.with_suffix("").with_suffix("")

    if not archive.exists():
        base_dir.mkdir(parents=True, exist_ok=True)
        with requests.get(url, stream=True) as r:
            r.raise_for_status()
            with open(archive, "wb") as f:
                for chunk in r.iter_content(8192):
                    f.write(chunk)

    if not extracted.exists():
        with tarfile.open(archive, "r:gz") as tar:
            tar.extractall(path=base_dir)

    input_dir = extracted / "bufr"
    if not output_dir.exists():
        output_dir.mkdir(parents=True, exist_ok=True)
        for bufr_file in input_dir.glob("*gdas*"):
            run_executable(OBS2IODA_V3_EXECUTABLE_PATH, "-i", str(input_dir), "-o", str(output_dir),
                           str(bufr_file.name))


NCEP_PREBUFR_BUFR_SUITE = TestSuite(
    marker="ncep_prepbufr_bufr",
    executable=Path(OBS2IODA_V3_EXECUTABLE_PATH),
    output_dir=Path.cwd() / "test/validation/data/v3/output",
    ref_dir=Path.cwd() / "test/validation/data/v3/reference",
    ext=".h5",
    fixture_name="ncep_prepbufr_bufr_file_pair",
    setup_fn=setup_validation,
)
