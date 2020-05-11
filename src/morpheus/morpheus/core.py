import json
import os
import pathlib
from pathlib import Path
import subprocess
import tempfile
from time import time
import typing as t

from cpuinfo import get_cpu_info_json
from progress.bar import Bar

from .analytics import TestResult
from .logger import logger
from .utils import file_len


class TestError(Exception):
    pass


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
    logger.debug(f"Testing program {program_path} space")

    tmpfile = tempfile.TemporaryFile()
    process = subprocess.Popen(
        str(program_path), stdin=input_path.open("r"), stdout=tmpfile
    )
    process.wait()
    tmpfile.flush()

    tmpfile.seek(0)
    line_size = len(tmpfile.readlines()) - 1
    tmpfile.seek(0, 2)
    byte_size = tmpfile.tell()
    return line_size, byte_size


def test_program(program_path: Path, input_path: Path, iterations=10) -> TestResult:
    logger.debug(f"Entered test_mapper")
    times = list()
    bar = Bar(f"Testing with file       {str(input_path.stem):15}", max=iterations)

    for i in range(iterations):
        result_time = test_time(program_path, input_path)
        times.append(result_time)
        bar.next()
    bar.finish()

    bar = Bar(f"Testing output size for {str(input_path.stem):15}", max=1)
    output_line_size, output_byte_size = test_space(program_path, input_path)
    bar.next()
    bar.finish()

    freq = int(json.loads(get_cpu_info_json())["hz_actual_raw"][0])

    result_data = TestResult(
        input_line_size=file_len(input_path),
        input_byte_size=input_path.lstat().st_size,
        output_line_size=output_line_size,
        output_byte_size=output_byte_size,
        times=times,
        avg_freq=freq,
        executable_path=program_path,
        test_file_path=input_path,
    )
    return result_data


def write_test_data(format_string: str, output_path: str):
    pass


def test_stage(
    program_path: pathlib.Path, program_type: str, input_path: pathlib.Path,
):
    input_files = []

    if input_path.is_dir():
        input_files.extend(input_path.glob("*"))
    else:
        input_files.append(input_path)

    results: t.List[TestResult] = list()
    for file in input_files:
        results.append(test_program(program_path, program_type, file))

    return results
