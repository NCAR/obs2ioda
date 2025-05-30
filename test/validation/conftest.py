import pytest
from pathlib import Path
from typing import Tuple, Any, Generator
from test_utils.goes_abi_suite import GOES_ABI_SUITE
from test_utils.ncep_prepbufr_bufr_suite import NCEP_PREBUFR_BUFR_SUITE
from test_utils.utils import get_pytest_markers

TEST_SUITES = {
    "ncep_prepbufr_bufr": NCEP_PREBUFR_BUFR_SUITE,
    "goes_abi": GOES_ABI_SUITE,
}

def pytest_configure(config):
    config.markers = [marker.split(":")[0] for marker in config.inicfg["markers"].splitlines()]

def pytest_generate_tests(metafunc: Any) -> None:
    for name, suite in TEST_SUITES.items():
        if name in get_pytest_markers(metafunc):
            suite.setup(metafunc)

@pytest.fixture(scope="session", autouse=True)
def teardown(request) -> Generator[None, Any, None]:
    yield
    for marker in request.config.markers:
        if marker in TEST_SUITES:
            TEST_SUITES[marker].teardown()

@pytest.fixture(scope="module")
def ncep_prepbufr_bufr_file_pair(request: Any) -> Tuple[Path, Path]:
    return request.param

@pytest.fixture(scope="module")
def goes_abi_file_pair(request: Any) -> Tuple[Path, Path]:
    return request.param
