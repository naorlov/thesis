from marshmallow import Schema
from marshmallow import fields

from .analytics import TestResult
from proto import Data_pb2

class StatsSchema(Schema):
    mean = fields.Float()
    stddev = fields.Float()
    median = fields.Float()


class TestResultSchema(Schema):
    data_length = fields.Integer()
    data_size = fields.Function(serialize=lambda x: x._test_file_path.stat().st_size)
    _executable = fields.String(data_key="executable")
    times = fields.List(fields.Float())
    stats = fields.Nested(StatsSchema())
    stats_95 = fields.Nested(StatsSchema())
    stats_99 = fields.Nested(StatsSchema())
    iterations = fields.Function(serialize=lambda x: len(x.times))
