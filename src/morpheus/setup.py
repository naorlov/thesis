from setuptools import setup


def read_file(filepath):
    with open(filepath, "r") as file:
        return file.read().split("\n")


install_requirements = read_file("requirements.txt")

setup(
    name="morpheus",
    version="0.0.1",
    license="MIT",
    author="Nikita Orlov",
    author_email="eaglebar@yandex.ru",
    packages=["src"],
    install_requirements=install_requirements,
    entry_points={
        "console_scripts": [
            "morpheus = src.__main__:main",
            "datagen = src.string_generator:main",
        ]
    },
)
