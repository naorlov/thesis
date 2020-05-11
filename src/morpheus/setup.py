from setuptools import setup


def read_file(filepath):
    with open(filepath, "r") as file:
        return file.read().split("\n")


# install_requirements = read_file("requirements.txt")

setup(
    name="morpheus",
    version="0.0.1",
    license="MIT",
    author="Nikita Orlov",
    author_email="eaglebar@yandex.ru",
    packages=["src", "morpheus"],
    # install_requirements=install_requirements,
    entry_points={
        "console_scripts": [
            "mrss-datagen = src.string_generator:main",
            "mrss-analyzer = morpheus.main:main",
        ]
    },
)
