from datetime import datetime
import logging
import os
import sys


def set_up_logging():
    """
        Setup the logging environment
    """
    filename = "logs/process_{date}.log".format(
        date=datetime.now().strftime("%Y-%m-%d")
    )
    if not os.path.exists(os.path.dirname(filename)):
        os.mkdir(os.path.dirname(filename))

    log = logging.getLogger()  # root logger
    log.setLevel(logging.INFO)

    format_str = "[%(asctime)s - %(levelname)8s] %(message)s"
    date_format = "%Y-%m-%d %H:%M:%S"

    formatter = logging.Formatter(format_str, date_format)

    stream_handler = logging.StreamHandler(sys.stderr)
    stream_handler.setFormatter(formatter)
    stream_handler.setLevel(logging.DEBUG)

    file_handler = logging.FileHandler(filename)
    file_handler.setFormatter(formatter)

    log.addHandler(stream_handler)
    log.addHandler(file_handler)
    return logging.getLogger("zmdb-gateway")


logger = set_up_logging()
