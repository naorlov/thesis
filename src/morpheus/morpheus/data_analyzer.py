from typing import List
from typing import Tuple

import numpy as np
from sklearn.linear_model import LinearRegression

from .analytics import TestResult

MAX_DEGREE = 15

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
        x = np.concatenate(
            list(x ** power for power in range(len(self.coef) // 2))
            + list(np.log(x) * x ** power for power in range(len(self.coef) // 2)),
            axis=1,
        )
        return self.trained_model.predict(x)

    @property
    def coef(self):
        return self.trained_model.coef_

    @property
    def intercept(self):
        return self.trained_model.intercept_


class DataAnalyzer:
    def __init__(self, test_results: List[TestResult]):
        self.test_results = test_results
        self.sizes = list(map(lambda result: result.data_length, self.test_results))
        self.mean_times = list(
            map(lambda result: result.stats_95.mean, self.test_results)
        )

    def _prepare_data(self, degree: int) -> Tuple[np.array, np.array]:
        x = np.array(self.sizes).reshape(-1, 1)
        y = np.array(self.mean_times).reshape(-1, 1)

        x = np.concatenate(
            list(x ** power for power in range(degree))
            + list(np.log(x) * x ** power for power in range(degree)),
            axis=1,
        )

        return x, y

    def find_model(self) -> ModelWrapper:
        if len(self.test_results) == 1:
            return None

        models = list()
        for degree in range(1, MAX_DEGREE + 1):
            X, y = self._prepare_data(degree)

            model = LinearRegression()
            model.fit(X, y)
            models.append((model.score(X, y), model))

        model = max(models, key=lambda t: t[0])

        return ModelWrapper(model)
