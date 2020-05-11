from pathlib import Path
from typing import List

import numpy as np


class TestResult:
    def __init__(
        self,
        input_line_size: int = None,
        input_byte_size: int = None,
        output_line_size: int = None,
        output_byte_size: int = None,
        times: List[float] = None,
        avg_freq: int = None,
        executable_path: Path = None,
        test_file_path: Path = None,
    ):
        self.input_line_size = input_line_size
        self.input_byte_size = input_byte_size
        self.output_line_size = output_line_size
        self.output_byte_size = output_byte_size
        self.times = np.array(times)
        self._analyzer = TimesAnalyzer(self.times)
        self.stats = self._analyzer.stats
        self.stats_95 = self._analyzer.stats_95
        self.stats_99 = self._analyzer.stats_99
        self._executable = executable_path.resolve()
        self._test_file_path = test_file_path.resolve()
        self.avg_freq = avg_freq
        self.flop = self.stats_95.mean * avg_freq

    def __str__(self):
        return (
            f"TestResult[length={self.input_line_size} "
            f"mean={self.stats_99.mean:.06f} "
            f"size={self.output_line_size}]"
        )

    __repr__ = __str__


class Stats:
    def __init__(
        self, mean=None, stddev=None, median=None,
    ):
        self.mean = mean
        self.stddev = stddev
        self.median = median

    def __repr__(self):
        return f"Stats[mean={self.mean}, stddev={self.stddev}, median={self.median}]"


class TimesAnalyzer:
    def __init__(self, time_list):
        self.time_list = np.array(time_list)

    @property
    def stats(self):
        return Stats(
            mean=np.mean(self.time_list),
            stddev=np.std(self.time_list),
            median=np.median(self.time_list),
        )

    @property
    def stats_95(self):
        return self._stats_below_percentile(95)

    @property
    def stats_99(self):
        return self._stats_below_percentile(99)

    def _stats_below_percentile(self, percentile):
        percentile_value = np.percentile(self.time_list, percentile)
        mask = self.time_list < percentile_value
        data = self.time_list[mask]
        return Stats(mean=np.mean(data), stddev=np.std(data), median=np.median(data))
