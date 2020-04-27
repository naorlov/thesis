import os
from pathlib import Path
from statistics import harmonic_mean
from statistics import mean
from statistics import median
from statistics import stdev
from time import time
import json
from .analytics import TestResult
from .logger import logger
from progress.bar import Bar
import subprocess
import tempfile

from cpuinfo import get_cpu_info_json


class TestError(Exception):
    pass


def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1


def test_time(program_path: Path, input_path: Path):
    logger.debug(f"entered test_once")
    start_time = time()
    empty_time = time() - start_time
    # TODO: Calibrate timers
    # TODO: Get info from perf counters

    start_time = time()
    result = os.system(f"{program_path} < {input_path} > /dev/null")
    result = time() - start_time - empty_time
    return result


def test_space(program_path: Path, input_path: Path):
    logger.debug("Testing program space")

    tmpfile = tempfile.TemporaryFile()
    process = subprocess.Popen(
        (program_path), stdin=input_path.open("r"), stdout=tmpfile
    )
    process.wait()
    tmpfile.flush()

    tmpfile.seek(0)
    result = len(tmpfile.readlines()) - 1

    return result


def test_mapper(program_path: Path, input_path: Path, iterations=10) -> TestResult:
    logger.debug(f"Entered test_mapper")
    times = list()
    bar = Bar(f"Testing with file       {str(input_path.stem):15}", max=iterations)
    for i in range(iterations):
        result_time = test_time(program_path, input_path)
        times.append(result_time)
        bar.next()
    bar.finish()

    bar = Bar(f"Testing output size for {str(input_path.stem):15}", max=1)
    output_size = test_space(program_path, input_path)
    bar.next()
    bar.finish()

    freq = int(json.loads(get_cpu_info_json())["hz_actual_raw"][0])
    result_data = TestResult(
        data_length=file_len(input_path),
        size=output_size,
        times=times,
        avg_freq=freq,
        executable_path=program_path,
        test_file_path=input_path,
    )
    return result_data


def test_reducer(program_path):
    pass


def test_program(program_path, type, input_path, count=10):
    if type == "mapper":
        return test_mapper(Path(program_path), Path(input_path), count)
    elif type == "reducer":
        return test_reducer(program_path)
    else:
        raise ValueError("Illegal type")


def write_test_data(format_string: str, output_path: str):
    pass
