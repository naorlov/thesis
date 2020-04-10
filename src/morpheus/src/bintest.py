from pathlib import Path


class Benchmark:
    def __init__(
        self, binary_path: str, input_path: str = None, input_template: str = None
    ):
        self._binary_path = Path(binary_path)
        self._input_path = Path(input_path) if input_path else Path()
        self._input_template = input_template

    def _execute(self, *args, **kwargs):
        pass

    def _build_input_data(self):
        if self._input_path:
            return self._input_path

        # generator = SimpleStringGenerator(self._input_template)
        # TODO: Sample data autogeneration

    def run(self):
        pass
