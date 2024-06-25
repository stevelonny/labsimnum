# Steve's Numerical Simulation Laboratory
_Stefano Lonardoni_

This is a collection of exercises written during Laboratorio di Simulazione Numerica (A.A. 2023-2024) @UNIMI. It's a pair of `C++` codes for data generation and `jupyter-notebook` for data analysis.

## Usage

### Dependences
For building and running the `C++` codes `cmake` and `make` are required, in addiction of a GCC compatible compiler (use gcc as clang does give problems beacuase of implicit narrowing and casting, tested with gcc 13.2 and 8.5). Dependencies linkage should be handled by CMake, but the user is required to provide them.

Data formatting in `c++`is done through [`fmtlib`](https://fmt.dev/latest/index.html) and it is provided through CMake.
Some exercises will use the [`armadillo`](https://arma.sourceforge.net/) libraries,`OpenMP` for dirty and fast speedup, and [exercise_10](#exercise-10) uses [`mpich`]() implementation of the MPI interaface.

All the data analysis is done through `jupyter-notebook` using as kernel python 3.12.2 with the following `python` modules: `mathplotlib`, `numpy`, `math`, `scipy` and for the last two exercises `tensorflow` on the kernel 3.9.

### Build & Run
Clone this repo. In the root directory create a folder called `build`. This is needed by `cmake` for storing the required configuration and `Makefile` files.

```shell
mkdir build
```

Navigate to the directory just created and run `cmake`:
```shell
cd build
cmake ..
```

Once finished, `cmake` will have configured the necessary `Makefile` for each exercise. The codes can now be compiled and run. Navigate to the desidered exercise's folder (inside the `build` directory) or compile all of the targets by running ```make``` in the `build` directory.

#### Installation
Targets installing the exercises are provided. Run the following command in the `build` directory:

```shell
make install
```
This will store all the exectubles inside the `bin` folder in the root of the project.

The data generated will be stored in a direcotry called `data`. The Jupyter notebooks will use this folder for the data analysis.

---

## Exercises

<!-- ### Global todo -->
<!-- 
- [x] Refactor all libraries (ex.: only one blocking avg)
- [x] Remake global cmake project structure like [this](https://cliutils.gitlab.io/modern-cmake/chapters/basics/structure.html)
- [x] Before making data public, uniform naming convention for all data files -->
<!-- - [x] Make data public for release -->
<!-- - [ ] ~~Implement auto-equilibration~~ -->
<!-- - [x] Investigate potential energy and total enry errors (too small !?) (investigate total energy)
- [ ] Add messages to user when launching programss -->

Exercise | Topic | Codes | Notebook
|:---|---|---|---|
| 01 | Pseudo-random numbers | [Codes](/exercise_01/) | [Notebook](/notebooks/exercise_01.ipynb) |
| 02 | Monte Carlo integration and random walks | [Codes](/exercise_02/) | [Notebook](/notebooks/exercise_02.ipynb) |
| 03 | Option pricing with Monte Carlo methods | [Codes](/exercise_03/) | [Notebook](/notebooks/exercise_03.ipynb) |
| 04 | Molecular dynamics | [Codes](/exercise_04/) | [Notebook](/notebooks/exercise_04.ipynb) |
| 05 | Quantum wavefunctions | [Codes](/exercise_05/) | [Notebook](/notebooks/exercise_05.ipynb) |
| 06 | Ising and Gibbs simulation | [Codes](/exercise_06/) | [Notebook](/notebooks/exercise_06.ipynb) |
| 07 | NVE vs NVT and autocorrelation | [Codes](/exercise_07/) | [Notebook](/notebooks/exercise_07.ipynb) |
| 08 | Quantum waveform sampling and simulated annealing | [Codes](/exercise_08/) | [Notebook](/notebooks/exercise_08.ipynb) |
| 09 | Genetic algorithm for TSP | [Codes](/exercise_09/) | [Notebook](/notebooks/exercise_09.ipynb) |
| 10 | MPI parallelization for TSP | [Codes](/exercise_10/) | [Notebook](/notebooks/exercise_10.ipynb) |
| 11 | Fitting functions with machine learning | nope | [Notebook, 01](/notebooks/exercise_11_1.ipynb), [Notebook, 02](/notebooks/exercise_11_2.ipynb), [Notebook, 03](/notebooks/exercise_11_3.ipynb) |
| 12 | Deep Neural Networks and CNN for image recognition | nope | [Notebook, 01](/notebooks/exercise_12_1.ipynb), [Notebook, 02 & 03](/notebooks/exercise_12_2_3.ipynb) |


<!-- 
### Exercise 01 ([Notebook](/notebooks/exercise_01.ipynb), [Codes](/exercise_01/))
Testing the pseudo-random generator with blocking averages and inverse distributions.

#### To Do
Done!
- [x] Accurate description in the notebook
- [x] Add assignments
- [x] Progressive uncertainity for all blocking avgs in the notebooks
- [x] Add fits to histograms in notebook, ex01.2
- [x] Commenting code where needed
- [x] Adding messages in the c++ codes

### Exercise 02 ([Notebook](/notebooks/exercise_02.ipynb), [Codes](/exercise_02/))
Monte Carlo integration and random walks.

#### To Do
Done!

- [x] Accurate description in the notebook
- [x] Add assignments
- [x] Add fits for random walks
- [x] Add comparison with fitted curves
- [x] Commenting code where needed
- [ ] Adding messages in the c++ codes

### Exercise 03 ([Notebook](/notebooks/exercise_03.ipynb), [Codes](/exercise_03/))
Option pricing with Monte Carlo methods.

#### To Do
Done!

- [x] Accurate description in the notebook
- [x] Add assignments
- [x] Add comparison between each method, and analytic resolutions
- [x] Commenting code where needed
- [ ] Adding messages in the c++ codes

### Exercise 04 ([Notebook](/notebooks/exercise_04.ipynb), [Codes](/exercise_04/))
First encounter with molecular dynamics. Pressure calculation and phase equilibration.

#### To Do
*CHECK TOTAL ENERGY FOR GASES*


instead of using omp for multithreading and a monolithic program, a python script may be more appropriate for the task.

- [x] Accurate description in the notebook (remain ex04.2)
- [x] Add assignments
- [x] Add intercept in jupyter with proposed values
- [ ] ~~Add controls to specify which phase to run~~
- [ ] ~~Polish unneeded output (for ex: each task in the equilibration step write to the same output file)~~

### Exercise 05 ([Notebook](/notebooks/exercise_05.ipynb), [Codes](/exercise_05/))
Sampling quantum wavefunctions with the Metropolis algorithm.

#### To Do
basic notebook (ex not required)
refactor code to use shared_ptr or similar. remember to use make_shared, and dynamic/static_pointer_cast.

### Exercise 06 ([Notebook](/notebooks/exercise_06.ipynb), [Codes](/exercise_06/))
Ising and Gibbs simulation.

#### To Do
* ADD OBSERVATIONS ON THE DIFFERENCE BETWEEN METRO AND GIBBS AT LOWER TEMPS *
accutate description in the notebook, add assignments, plots and fits

### Exercise 07 ([Notebook](/notebooks/exercise_07.ipynb), [Codes](/exercise_07/))
NVE vs NVT and their autocorrelation.

#### To Do
accurate descr, ~~add assignments, refactor autocorrelation compute. NEEDS GOFRRRRR~~, add observations for comparisons and negative values in autocorrelation?


### Exercise 08 ([Notebook](/notebooks/exercise_08.ipynb), [Codes](/exercise_08/))
Quantum waveform sampling and simulated annealing with custom potential.

#### To Do
complete the observations ant theory part, power spectrum???, ~~add assignments,to do everything after refactor ex05 code. rewrite code to borrowing standr or similar~~

### Exercise 09 ([Notebook](/notebooks/exercise_09.ipynb), [Codes](/exercise_09/))
Genetic algorithm for TSP.


#### To Do
accurate descr, add assignments
(added better comparison between selections)
~~check every mutation algorithm, and find best mutation rate.~~


### Exercise 10 ([Notebook](/notebooks/exercise_10.ipynb), [Codes](/exercise_10/))
MPI parallelization with migrations for TSP.

#### To Do
accurate descr, add assignments
refactor atlas so we can send with MPI_Bcast the map to all nodes, or at least a seed system (should be already implentem implicitely beacause rnd set seed with 0)

### Exercise 11 ([Notebook, 01](/notebooks/exercise_11_1.ipynb), [Notebook, 02](/notebooks/exercise_11_2.ipynb), [Notebook, 03](/notebooks/exercise_11_3.ipynb))
Fitting functions with machine learning.

### Exercise 12 ([Notebook, 01](/notebooks/exercise_12_1.ipynb), [Notebook, 02 & 03](/notebooks/exercise_12_2_3.ipynb))
Deep Neural Networks and CNN for image recognition. -->

---
---


## License

This project is licensed under the [MIT License](LICENSE).
