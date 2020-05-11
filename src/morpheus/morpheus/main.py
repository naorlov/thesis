import pathlib
import typing as t

import click
import yaml

from .data_analyzer import DataAnalyzer
from .pipeline import TestingPipeline


@click.group()
def main():
    pass


def validate_specfile(spec: dict):
    for name, description in spec["stages"].items():
        if not pathlib.Path(description["executable"]).exists():
            raise FileNotFoundError(description["executable"])
        if not pathlib.Path(description["input"]).exists():
            raise FileNotFoundError(description["input"])


def read_specfile(specfile_path: pathlib.Path):
    """
    Reads specfile and performs some manipulations with paths.

    If file is invalid or paths does not exist, raises ValueError with description
    :param specfile_path:
    :return:
    """
    specfile = yaml.load(specfile_path.read_text(), Loader=yaml.FullLoader)
    stages = specfile["stages"]
    for name, description in stages.items():
        description["executable"] = (
            (specfile_path.parent / pathlib.Path(description["executable"]))
            .expanduser()
            .resolve()
        )
        description["input"] = (
            (specfile_path.parent / pathlib.Path(description["input"]))
            .expanduser()
            .resolve()
        )

    # If file is invalid, then it raises exception that must be caught in caller
    validate_specfile(specfile)
    return specfile


@main.command()
@click.option(
    "-s",
    "--specfile",
    help="MRSS specification file",
    type=pathlib.Path,
    required=True,
)
@click.option(
    "--no-explode",
    help="If set program will not perform file explosion as described in doc",
    is_flag=True,
)
def full(specfile: pathlib.Path, no_explode: bool):
    document = read_specfile(specfile)
    stages: t.Dict[str, t.Dict[str, str]] = document["stages"]

    global_result = {}

    for name, description in stages.items():
        program_path = (
            (specfile.parent / pathlib.Path(description["executable"]))
            .expanduser()
            .resolve()
        )
        input_path = (
            (specfile.parent / pathlib.Path(description["input"]))
            .expanduser()
            .resolve()
        )
        testing_pipeline = TestingPipeline(
            program_path, description["type"], input_path
        )
        result = testing_pipeline.run()
        analyzer = DataAnalyzer(result)
        time_model = analyzer.find_line_time_model()
        space_model = analyzer.find_space_model()

        global_result[name] = {
            "time": time_model.to_json(),
            "space": space_model.to_json(),
        }

    (specfile.parent / "result.yaml").write_text(yaml.dump(global_result))
