import json
from pathlib import Path
from typing import List

from src.analytics import TestResult
from src.core import test_program
from src.data_analyzer import DataAnalyzer
from src.models import TestResultSchema


class CLI:
    def __init__(
        self,
        executable_path: Path,
        type: str = None,
        input_path: Path = None,
        input_template: str = None,
        test_result_path: Path = None,
        count: int = None,
    ):
        self.executable_path = executable_path
        self.type = type
        self.input_path = input_path
        self.input_template = input_template
        self.test_result = test_result_path
        self.count = count

        if not any((self.input_path, self.input_template)):
            raise ValueError("input-path or input-template arguments must be providen")

    def get_input_files(self) -> List[Path]:
        if self.input_path.is_dir():
            test_files = list(self.input_path.glob("*"))
        else:
            test_files = [self.input_path]

        return test_files

    def benchmark_over(self, test_files) -> List[TestResult]:
        results: List[TestResult] = list()
        for file in test_files:
            results.append(
                test_program(self.executable_path, self.type, file, self.count)
            )
        return results

    def analyze(self, results):
        analyzer = DataAnalyzer(results, "linear")
        model = analyzer.find_model()
        return model

    def run(self):
        test_files = self.get_input_files()

        results: List[TestResult] = self.benchmark_over(test_files)

        output = TestResultSchema().dump(results, many=True)

        if not self.test_result:
            print(json.dumps(output, indent=4, sort_keys=True))

        result_model = self.analyze(results)
