# Steve's Numerical Simulation Laboratory

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)

## Description

This is a collection of exercises written during Laboratorio di Simulazione Numerica (A.A. 2023-2024) @UNIMI. It's a pair of `C++` codes for data generation and `jupyter-notebook` for data analysis.

## Installation & Usage

### Dependences
For building and running the `C++` codes `cmake` and `make` are required, in addiction of a gcc/g++ (clang does give problems compiling some libraries). Dependencies should be handled by cmake.

Data formatting in `c++`is done through [`fmtlib`](https://fmt.dev/latest/index.html) and it is provided within the repository.
Exercise 04 will use the [`armadillo`](https://arma.sourceforge.net/) libraries for linear algebra calculations in the simulator provided, and `ex04_eq.cpp` make use of multithreading with `OpenMP`.

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

The data generated will be stored in a direcotry called `data`. The Jupyter notebooks will use this folder for the data analysis.

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

#### To Do

- [ ] Accurate description in the notebook
- [ ] Add assignments
- [ ] Add fits for random walks
- [ ] Add comparison with fitted curves
- [x] Commenting code where needed
- [ ] Adding messages in the c++ codes

### Exercise 03
[Notebook](/notebooks/exercise_03.ipynb), [Codes](/exercise_03/)

#### To Do

- [ ] Accurate description in the notebook
- [ ] Add assignments
- [ ] Add comparison between each method, and analytic resolutions
- [ ] Commenting code where needed
- [ ] Adding messages in the c++ codes

### Exercise 04

[Notebook](/notebooks/exercise_04.ipynb), [Codes](/exercise_04/)

This exercise is divided in three parts:
- Equilibration - Generate a bunch of temperatures data to extrapolate starting temperature needed to equilibrate
- Preparation - Prepare the system in a equilibrated state
- Simulation - Run the actual simulation

#### exercise_04_eq - _Equilibration_
This program runs 10 simulation for each of the 3 phases at different temperature. Its results are used for estimating equilibration time and starting temperature necessary to achieve the desidered temperature.
On a quadcore i5-10300H, with 100 blocks and 200 steps each block the speed comparison is:
*Single-thread*
```
real    15m59.719s
user    15m55.893s
sys     0m0.335s
```
*Multi-thread*
```
real    3m30.244s
user    21m48.532s
sys     0m0.909s
```
Todo: get rid of output as it is not needed.

#### exercise_04_prep - _Preparation_
This program runs simulations to prepare a stable configuration for the desired configurations.
Run 1 single block of the required number of steps (solid: 50000, liquid: 50000, gas: 500000). Saves the output in [data/ex04](/data/ex04/) under ```INPUT_*```.

#### exercise_04_sim - _Simulation_
Runs the actual simulation restarting from the input folders generated from the previous code.

#### To Do

- [ ] Accurate description in the notebook
- [ ] Add assignments
- [x] Add intercept in jupyter with proposed values
- [ ] Add a way to calculate equilibration time (for now it is estimated)
- [ ] Add controls to specify which phase to run
- [ ] Polish unneeded output (for ex: each task in the equilibration step write to the same output file)

### Exercise 05

[Notebook](/notebooks/exercise_05.ipynb), [Codes](/exercise_05/)

TODO: eliminate usage of smart ptrs as their correct implementations request a complete refactor of constructors

### Exercise 06

Usage:

    ./exercise_06 <simulation_type> [<external_field>]

Where ```<simulation_type>``` cane be either ```2``` for metro or ```3``` for gibbs.
Without ```[<external_field>]``` set, the program computes tenergy, heat capacity and chi with ```h = 0```.
The optional argument ```[<external_field>]``` can be whatever value, it will prompt the program to set the external field to ```h = 0.02``` and it will compute only magnetization values.

TODO: investigate strange behaviour when taking advantage of omp directives, such outputting certain values to 0.


---
---

## License

This project is licensed under the [MIT License](LICENSE).
