import json
from pathlib import Path

from src.core import test_program


class CLI:
    def __init__(
        self,
        executable_path: Path,
        type: str = None,
        input_path: Path = None,
        input_template: str = None,
        test_result_path: Path = None,
    ):
        self.executable_path = executable_path
        self.type = type
        self.input_path = input_path
        self.input_template = input_template
        self.test_result = test_result_path

        if not any((self.input_path, self.input_template)):
            raise ValueError("input-path or input-template arguments must be providen")

    def run(self):
        input_path: Path = self.input_path

        if input_path.is_dir():
            test_files = list(input_path.glob("*"))
        else:
            test_files = [input_path]

        results = list()
        for file in test_files:
            results.append(test_program(self.executable_path, self.type, file))

        if not self.test_result:
            print(json.dumps(results, indent=4))
            return

        with self.test_result.open("w") as file:
            json.dump(results, file, indent=4)
