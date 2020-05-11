import pathlib

from morpheus.core import test_program
from morpheus.utils import get_input_files


class TestingPipeline:
    def __init__(
        self, program_path: pathlib.Path, program_type: str, input_path: pathlib.Path,
    ):
        self._program_path = program_path
        self._program_type = program_type
        self._input_path = input_path

        self._input_files = sorted(
            get_input_files(input_path), key=lambda file: file.line_size
        )

        self._build_pipeline()

    def _build_pipeline(self):
        max_line_size = max(map(lambda file: file.line_size, self._input_files))
        max_byte_size = max(map(lambda file: file.byte_size, self._input_files))

    def run(self):
        results = []
        for file in self._input_files:
            results.append(test_program(self._program_path, file.path))
        return results
