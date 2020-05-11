import typing as t

import numpy as np
from sklearn.linear_model import LinearRegression
from sklearn.model_selection import train_test_split

from .analytics import TestResult

MAX_DEGREE = 15


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
    def __init__(self, test_results: t.List[TestResult]):
        self.test_results = test_results
        self.input_line_sizes = list(
            map(lambda result: result.input_line_size, self.test_results)
        )
        self.input_byte_sizes = list(
            map(lambda result: result.output_line_size, self.test_results)
        )
        self.mean_times = list(
            map(lambda result: result.stats_95.mean, self.test_results)
        )
        self.output_line_sizes = list(
            map(lambda result: result.output_line_size, self.test_results)
        )
        self.output_byte_sizes = list(
            map(lambda result: result.output_byte_size, self.test_results)
        )
        self.flops = list(map(lambda result: result.flop, self.test_results))

    def _prepare_data(self, x, y, degree: int) -> t.Tuple[np.array, np.array]:
        x = np.array(x).reshape(-1, 1)
        y = np.array(y).reshape(-1, 1)
        x = np.concatenate(
            list(x ** power for power in range(degree + 1))
            + list(np.log(x) * (x ** power) for power in range(degree + 1)),
            axis=1,
        )
        return x, y

    def find_line_time_model(self) -> t.Optional[ModelWrapper]:
        return self.find_model(self.input_line_sizes, self.mean_times)

    def find_line_space_model(self) -> t.Optional[ModelWrapper]:
        return self.find_model(self.input_line_sizes, self.output_line_sizes)

    def find_byte_time_model(self) -> t.Optional[ModelWrapper]:
        return self.find_model(self.input_byte_sizes, self.mean_times)

    def find_byte_space_model(self) -> t.Optional[ModelWrapper]:
        return self.find_model(self.input_byte_sizes, self.output_byte_sizes)

    def find_model(self, x, y) -> t.Optional[ModelWrapper]:
        if len(self.test_results) == 1:
            return None

        models = list()
        for degree in range(1, MAX_DEGREE + 1):
            x, y = self._prepare_data(x, y, degree)
            x_train, x_test, y_train, y_test = train_test_split(x, y, test_size=0.25)
            model = LinearRegression(fit_intercept=False)
            model.fit(x_train, y_train)
            models.append((model.score(x_test, y_test), model))

        model = max(models, key=lambda m: m[0])

        return ModelWrapper(model[1])
