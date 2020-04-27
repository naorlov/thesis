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
            + list(np.log(x) * (x ** power) for power in range(len(self.coef) // 2)),
            axis=1,
        )
        return self.trained_model.predict(x)

    @property
    def coef(self):
        return self.trained_model.coef_

    @property
    def intercept(self):
        return self.trained_model.intercept_

    def __str__(self):
        degree = len(self.coef[0]) // 2
        return f"ModelWrapper[degree={degree}, ]"

    def to_json(self):
        degree = len(self.coef[0]) // 2
        result = {
            "degree": degree,
            "polynomial_part": list(map(float, list(self.coef[0])[:degree])),
            "logarithmic_part": list(map(float, list(self.coef[0])[degree:])),
        }
        return result


class DataAnalyzer:
    def __init__(self, test_results: List[TestResult]):
        self.test_results = test_results
        self.sizes = list(map(lambda result: result.data_length, self.test_results))
        self.mean_times = list(
            map(lambda result: result.stats_95.mean, self.test_results)
        )
        self.output_sizes = list(map(lambda result: result.size, self.test_results))
        self.flops = list(map(lambda result: result.flop, self.test_results))

    def _prepare_data(self, data, degree: int) -> Tuple[np.array, np.array]:
        # print("=" * 80)
        x = np.array(self.sizes).reshape(-1, 1)
        y = np.array(data).reshape(-1, 1)
        # print("-" * 80)
        # print(x)
        x = np.concatenate(
            list(x ** power for power in range(degree + 1))
            + list(np.log(x) * (x ** power) for power in range(degree + 1)),
            axis=1,
        )
        # print(x)
        return x, y

    def find_time_model(self) -> ModelWrapper:
        if len(self.test_results) == 1:
            return None

        models = list()
        for degree in range(1, MAX_DEGREE + 1):
            X, y = self._prepare_data(self.flops, degree)

            model = LinearRegression(fit_intercept=False)
            model.fit(X, y)
            models.append((model.score(X, y), model))

        print(
            *[
                f"score = {model[0]} degree = {len(model[1].coef_[0]) // 2} \n"
                for model in models
            ]
        )

        # TODO: Add cross-validation
        # TODO: Generate more data
        model = models[0]

        return ModelWrapper(model[1])

    def find_space_model(self) -> ModelWrapper:
        if len(self.test_results) == 1:
            return None

        models = list()
        for degree in range(1, MAX_DEGREE + 1):
            X, y = self._prepare_data(self.output_sizes, degree)

            model = LinearRegression()
            model.fit(X, y)
            models.append((model.score(X, y), model))

        model = max(models, key=lambda t: t[0])

        return ModelWrapper(model[1])
