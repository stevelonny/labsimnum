# Numerical Simulation Laboratory

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## Description

This is a collection of exercises written during Laboratorio di Simulazione Numerica (A.A. 2023-2024) @UNIMI. It's a pair of `C++` codes for data generation and `jupyter-notebook` for data analysis.

## Installation & Usage

### Dependences
For building and running the `C++` codes `cmake` and `make` are required, in addiction of a `C++` compiler such as gcc/g++ or clang version `c++17` or later. Data formatting in `c++`is done through [`fmtlib`](https://fmt.dev/latest/index.html) and it is provided within the repository.

All the data analysis is done through `jupyter-notebook` using as kernel python 3.12.2 with the following `python` modules: `mathplotlib`, `numpy`, `math`.

### Build
Clone this repo. In the root directory create a folder called `build`. This is needed by `cmake` for storing the required configuration and `Makefile` files.

```shell
mkdir build
```

Navigate to the directory just created and run `cmake`:
```shell
cd build
cmake ..
```

Once finished, `cmake` will have configured the necessary `Makefile` for each exercise. The codes can now be compiled and run. Navigate to the desidered exercise's folder (inside the `build` directory) or compile all of the targets by running in the `build` directory.

The data generated will be stored in a direcotry called `data`. Create such directory in the root of the project. The Jupyter notebooks will use this folder for the data analysis.

### Usage

_TODO_

## Exercises

_TODO_

### Global todo

- [ ] Refactor all libraries (ex.: only one blocking avg)
- [ ] Remake global cmake project structure like [this](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html)
- [ ] Make data public for release

### Exercise 01
[Notebook](/notebooks/exercise_01.ipynb), [Codes](/exercise_01/)

#### To Do

- [ ] Accurate description in the notebook
- [x] Add assignments
- [x] Progressive uncertainity for all blocking avgs in the notebooks
- [ ] Add fits to histograms in notebook, ex01.2
- [x] Commenting code where needed
- [x] Adding messages in the c++ codes

### Exercise 02
[Notebook](/notebooks/exercise_02.ipynb), [Codes](/exercise_02/)

### To Do

- [ ] Accurate description in the notebook
- [ ] Add assignments
- [ ] Add fits for random walks
- [ ] Add comparison with fitted curves
- [x] Commenting code where needed
- [ ] Adding messages in the c++ codes

### Exercise 03
[Notebook](/notebooks/exercise_03.ipynb), [Codes](/exercise_03/)

### To Do

- [ ] Accurate description in the notebook
- [ ] Add assignments
- [ ] Add comparison between each method, and analytic resolutions
- [ ] Commenting code where needed
- [ ] Adding messages in the c++ codes


## License

This project is licensed under the [MIT License](LICENSE).
