import logging
from src import logger
from src.cli import CLI
from .arguments import parser


def main():
    args = parser.parse_args()

    if args.verbose:
        logger.logger.setLevel(level=logging.DEBUG)
    else:
        logger.logger.setLevel(level=logging.ERROR)

    cli = CLI(
        executable_path=args.executable,
        type=args.type,
        input_path=args.input_path,
        input_template=args.input_template,
        test_result_path=args.test_result_path,
        count=args.count,
    )
    cli.run()


if __name__ == "__main__":
    main()
