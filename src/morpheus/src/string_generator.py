import argparse
from pathlib import Path
import random
import re
import string

regexp = re.compile("{[a-zA-Z0-9:-]*}")


def random_string(length: str = None):
    """

    :param length: Длина строки
    :return: Случайная строка
    """

    return "".join(random.choices(string.ascii_letters, k=int(length)))


def random_int(lower: str = None, upper: str = None):
    """

    :param lower:
    :param upper:
    :return:
    """
    if lower is None:
        lower = 0
    else:
        lower = int(lower)
    if upper is None:
        upper = 100
    else:
        upper = int(upper)

    return random.randint(lower, upper)


def random_float(lower: str = None, upper: str = None):
    """

    :param lower:
    :param upper:
    :return:
    """
    if lower is None:
        lower = 0
    else:
        lower = float(lower)
    if upper is None:
        upper = 100
    else:
        upper = float(upper)

    return random.random() * (upper - lower) + lower


format_handlers = {
    "string": random_string,
    "int": random_int,
    "float": random_float,
}


class SimpleStringGenerator:
    def __init__(self, template_string):
        self._template_string = template_string

    def _render_once(self):
        """
        http://{string:10}.com {int} {float} {float:3-5} delta{string:10}{int}
        """

        match = regexp.search(self._template_string)
        if not match:
            return self._template_string

        result_string = self._template_string[: match.start()]

        while match:
            format_arg = self._template_string[match.start() : match.end()]
            clean_arg = format_arg[1:-1]

            format_arguments = clean_arg.split(":")

            if len(format_arguments) == 1:
                format_name = format_arguments[0]
                added_substring = format_handlers[format_name]()
            elif len(format_arguments) == 2:
                format_name, format_params = format_arguments
                format_params = format_params.split("-")
                added_substring = format_handlers[format_name](*format_params)
            else:
                raise ValueError()

            prev_end = match.end()
            match = regexp.search(self._template_string, prev_end)
            next_start = match.start() if match else None
            result_string += (
                str(added_substring) + self._template_string[prev_end:next_start]
            )

        return result_string

    def render(self, count: int = 1):
        result = list()
        for _ in range(count):
            result.append(self._render_once())

        if count == 1:
            return result[0]
        return result


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("format_string")
    parser.add_argument("output_file", type=Path)
    parser.add_argument("count", type=int)

    args = parser.parse_args()

    generator = SimpleStringGenerator(args.format_string)

    with open(args.output_file, "w") as file:
        result = generator.render(args.count)
        file.write("\n".join(result))
