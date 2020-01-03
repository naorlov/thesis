import json
from pathlib import Path

from src.cli import CLI
from .arguments import parser
from .core import test_program


def main():
    args = parser.parse_args()

    cli = CLI(
        executable_path=args.executable,
        type=args.type,
        input_path=args.input_path,
        input_template=args.input_template,
        test_result_path=args.test_result_path,
    )
    cli.run()


if __name__ == "__main__":
    main()
