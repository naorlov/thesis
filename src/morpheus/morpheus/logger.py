from datetime import datetime
import logging
import os
from pathlib import Path
import sys

LOGLEVEL = os.environ.get("LOGLEVEL", "INFO").upper()


def set_up_logging():
    """
        Setup the logging environment
    """
    filename = Path(
        "~/.mrss/analyzer/logs/process_{date}.log".format(
            date=datetime.now().strftime("%Y-%m-%d_%H:%M:%S")
        )
    ).expanduser()

    if not filename.parent.exists():
        filename.parent.mkdir(parents=True)

    log = logging.getLogger()  # root logger
    log.setLevel(LOGLEVEL)

    format_str = (
        "[%(asctime)s - %(levelname)-8s "
        "%(filename)18s:%(lineno)-4s - %(funcName)32s() ] %(message)s"
    )
    date_format = "%Y-%m-%d %H:%M:%S"

    formatter = logging.Formatter(format_str, date_format)

    stream_handler = logging.StreamHandler(sys.stderr)
    stream_handler.setFormatter(formatter)
    stream_handler.setLevel(logging.DEBUG)

    file_handler = logging.FileHandler(filename)
    file_handler.setFormatter(formatter)

    log.addHandler(stream_handler)
    log.addHandler(file_handler)
    return logging.getLogger("mrss")


logger = set_up_logging()
