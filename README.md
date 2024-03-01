# Numerical Simulation Laboratory

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## Description

This is a collection of exercises written during Laboratorio di Simulazione Numerica (A.A. 2023-2024) @UNIMI. It's a pair of `C++` codes for data generation and `jupyter-notebooks` for data analysis.

## Installation & Usage

### Dependences
For building and running the `C++` codes, `cmake` and `make` are required, in addiction of a `C++` compiler such as gcc/g++ or clang. All the data analysis is done through `jupyter-notebooks`, using the following `python` modules: `mathplotlib`, `numpy`, `math`. 

### Build
After having cloned this repo, in its root directory create a directory called `build`, where `cmake` will create the required configuration files.

```shell
mkdir build
```
Navigate to the directory just created and run `cmake`
```shell
cd build
cmake ..
```
Once finished, `cmake` configured the necessary `Makefile` for each exercise. The codes can now be compiled and run, by navigating to the desidered exercise's folder (inside the `build` directory)mor compiling all of the targets.

These codes will store all the data generated in a folder called `data`, which has to be created in the root directory of the project. The Jupyter notebooks will use this folder for the data analysis.

### Usage

_TODO_

## Exercises

_TODO_

## License

This project is licensed under the [MIT License](LICENSE).
