from pathlib import Path
from typing import Any

from typing_extensions import Callable

from .utils import clean_directory, parametrize_file_pairs


class TestSuite:
    def __init__(
            self,
            marker: str,
            executable: Path,
            output_dir: Path,
            ref_dir: Path,
            ext: str,
            fixture_name: str,
            setup_fn: Callable[[Path], None] = None
    ):
        self.marker = marker
        self.executable = executable
        self.output_dir = output_dir
        self.ref_dir = ref_dir
        self.ext = ext
        self.fixture_name = fixture_name
        self.setup_fn = setup_fn

    def setup(self, metafunc: Any):
        if self.setup_fn:
            self.setup_fn(self.output_dir)
        parametrize_file_pairs(metafunc, self.fixture_name, self.ref_dir, self.output_dir, self.ext)

    def teardown(self):
        clean_directory(self.output_dir)