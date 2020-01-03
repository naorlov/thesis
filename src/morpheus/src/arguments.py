import argparse
from pathlib import Path

parser = argparse.ArgumentParser(
    prog="morpheus", description="Program to perform binary testing",
)

parser.add_argument("executable", help="command to test")

parser.add_argument("--type", choices=["mapper", "reducer"])
parser.add_argument("--input-path", type=Path)
parser.add_argument("--input-template", type=str)
parser.add_argument("--test-result-path", type=Path)
