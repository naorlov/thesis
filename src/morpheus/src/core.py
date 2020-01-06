import os
from pathlib import Path
from statistics import harmonic_mean
from statistics import mean
from statistics import median
from statistics import stdev
from time import time

from .analytics import TestResult
from .logger import logger


class TestError(Exception):
    pass


def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1


def test_once(program_path: Path, input_path: Path):
    start_time = time()
    empty_time = time() - start_time
    # TODO: Calibrate timers
    # TODO: Get info from perf counters

    start_time = time()
    result = os.system(f"{program_path} < {input_path} > /dev/null")
    result = time() - start_time - empty_time
    return result


def test_mapper(program_path: Path, input_path: Path, iterations=10) -> TestResult:
    logger.info(
        f"Testing {program_path.resolve().stem} on {input_path.resolve().stem},"
        f" line count = {file_len(input_path)}"
    )
    times = list()
    for i in range(iterations):
        logger.info(f"Test iteration {i}")
        result_time = test_once(program_path, input_path)
        times.append(result_time)
        logger.info(f"Took {result_time}")

    result_data = {
        "data_length": file_len(input_path),
        "times": times,
        "stddev": stdev(times),
        "mean": mean(times),
        "harmonic_mean": harmonic_mean(times),
        "median": median(times),
    }

    result_data = TestResult(
        data_length=file_len(input_path),
        times=times,
        executable_path=program_path,
        test_file_path=input_path,
    )
    return result_data


def test_reducer(program_path):
    pass


def test_program(program_path, type, input_path, count=None):
    if type == "mapper":
        return test_mapper(Path(program_path), Path(input_path), count)
    elif type == "reducer":
        test_reducer(program_path)
    else:
        raise ValueError("Illegal type")


def write_test_data(format_string: str, output_path: str):
    pass
