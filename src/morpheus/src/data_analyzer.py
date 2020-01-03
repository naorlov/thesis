from typing import List

import numpy as np
from sklearn.linear_model import LinearRegression

from src.analytics import TestResult


# _monoms = [
#     lambda data: np.ones_like(data),  # constant term
#     lambda data: data,  # linear term
#     lambda data: np.log(data),  # log term
#     lambda data: np.log(np.log(data)),  # repeated log term
#     lambda data: np.power(data, 2),  # square
#     lambda data: np.power(data, 2),
# ]

class ModelWrapper:
    def __init__(self, trained_model: LinearRegression):
        self.trained_model = trained_model

    def __call__(self, data_size):
        x = np.array([data_size])
        return self.trained_model.predict(x)

    @property
    def coef(self):
        return self.trained_model.coef_


class DataAnalyzer:
    def __init__(self, test_results: List[TestResult], mode: str = None):
        self.test_results = test_results
        self.mode = mode

        if self.mode != "linear":
            raise ValueError("Non-linear modes are not currently supported")

    def _prepare_data(self, x: List[float], y: List[float]):
        x = np.array(x).reshape(-1, 1)
        y = np.array(y).reshape(-1, 1)
        return x, y

    def find_model(self):
        if len(self.test_results) == 1:
            return None

        sizes = list(map(lambda result: result.data_length, self.test_results))
        mean_times = list(map(lambda result: result.stats_95.mean, self.test_results))

        model = LinearRegression()

        X, y = self._prepare_data(sizes, mean_times)

        model.fit(X, y)

        return ModelWrapper(model)
