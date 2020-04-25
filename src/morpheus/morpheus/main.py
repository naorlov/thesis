import click
import pathlib
import yaml
import typing as t
import random
from .core import test_program
from .core import file_len
from .analytics import TestResult
from .data_analyzer import DataAnalyzer
from .logger import logger
import math


@click.group()
def main():
    pass


def validate_specfile(spec):
    pass


def sample_file(input_path: pathlib.Path, file_length: int, result_size: int) -> str:
    logger.debug(f"{input_path=} {file_length=} {result_size=}")
    result = []
    selection_indecies = iter(sorted(random.sample(range(file_length), result_size)))
    current_selected_idx = next(selection_indecies)
    for idx, line in enumerate(input_path.read_text()):
        if idx == current_selected_idx:
            result.append(line)
            try:
                current_selected_idx = next(selection_indecies)
            except:
                break
    return "\n".join(result)


def break_up_input(input_path: pathlib.Path):
    parent_dir = input_path.parent
    line_count = file_len(input_path)

    if line_count < 1e8:
        # TODO: make file bigger for testing purposes
        pass

    sizes = [1000 ** power for power in range(int(math.log10(line_count) - 3))] + [
        5000 ** power for power in range(int(math.log10(line_count) - 3))
    ]

    for size in sizes:
        data = sample_file(input_path, line_count, size)
        current_path = parent_dir / f"{input_path.stem}_{size}"
        current_path.write_text(data)


def test_stage(
    program_path: pathlib.Path, program_type: str, input_path: pathlib.Path,
):
    input_files = []

    if input_path.is_dir():
        input_files.extend(input_path.glob("*"))
    else:
        input_files.append(input_path)

    results: List[TestResult] = list()
    for file in input_files:
        results.append(test_program(program_path, program_type, file))

    return results


@main.command()
@click.option(
    "-sf",
    "--specfile",
    help="MRSS specification file",
    type=pathlib.Path,
    required=True,
)
def full(specfile):
    document = yaml.load(specfile.read_text(), Loader=yaml.FullLoader)
    validate_specfile(document)
    stages: t.Dict[str, t.Dict[str, str]] = document["stages"]

    for name, descriprion in stages.items():
        program_path = (
            (specfile.parent / pathlib.Path(descriprion["executable"]))
            .expanduser()
            .resolve()
        )
        input_path = (
            (specfile.parent / pathlib.Path(descriprion["input"]))
            .expanduser()
            .resolve()
        )
        result = test_stage(
            program_path=program_path,
            program_type=descriprion["type"],
            input_path=input_path,
        )
        analyzer = DataAnalyzer(result)
        reuslt = analyzer.find_model()
        
        