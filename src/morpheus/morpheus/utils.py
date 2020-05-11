import math
import pathlib
import random
import typing as t

from attr import dataclass


@dataclass
class TestInput:
    path: pathlib.Path
    byte_size: int
    line_size: int


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


def sample_file(input_path: pathlib.Path, file_length: int, result_size: int) -> str:
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


def get_input_files(input_path: pathlib.Path) -> t.List[TestInput]:
    result = []
    if input_path.is_dir():
        for file in input_path.glob("*"):
            result.append(
                TestInput(
                    path=file, byte_size=file.lstat().st_size, line_size=file_len(file)
                )
            )
    else:
        result.append(
            TestInput(
                path=input_path,
                byte_size=input_path.lstat().st_size,
                line_size=file_len(input_path),
            )
        )

    return result


def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1
