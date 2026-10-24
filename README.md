# ML Infrastructure Framework in C++

A from-scratch machine-learning framework written in native C++. It covers the supervised-learning pipeline for **linear regression** end to end: CSV loading, shuffled train/test splitting, min–max scaling, batch gradient descent, evaluation (MSE, R²), model persistence and Gnuplot plotting.

The only external pieces are the compiler's **OpenMP** runtime (for the parallel kernels) and the optional **Gnuplot** program (for plots). No third-party machine-learning or linear-algebra libraries are used.

This code accompanies the paper *"A Thread-Safe and Numerically Stable End-to-End Machine Learning Framework in Native C++"* (Chirag Solanki, Ritika Bhatt; Acropolis Institute of Technology and Research, Indore).

## What is implemented

| Area | Components |
|---|---|
| Linear algebra | `matrix` class (row-major `std::vector<double>`, bounds-checked `m[r][c]`, lazy identity matrix, copy/move semantics, overloaded operators) and `matrix_math` (OpenMP kernels: add, subtract, multiply, divide, matrix product) |
| Data I/O | `csv::load_x_y` (POSIX I/O, adds a bias column of 1.0, last column is the target), `dataset_utils::split` (shuffled split into two matrices or two CSV files) |
| Preprocessing | `scaler::min_max_scaler` (fit on training data, re-applied to test data), `encoder::encode_one_hot` |
| Model | `linear_regression::batch_gradient_fit` (batch gradient descent, cost J = MSE/2, optional per-cycle callback) |
| Evaluation | `ml_math::mean_squared_error`, `ml_math::r2score` |
| Utilities | `random_engine::fill` (uniform random initialisation), `plotter` (Gnuplot through a pipe), model / cost / min–max saved as CSV |

### Work in progress (declared but not finished)

`standard_scaler`, robust scaler, mini-batch and stochastic gradient descent, L1 / L2 / Elastic-Net regularisation (`regularization_parameter` is accepted but not yet applied), cost functions other than MSE, target / ordinal encoders, `column_cutter` (selects columns but does not write the output file yet).

## Requirements

* `g++` with C++17 and OpenMP support (`-fopenmp`)
* Gnuplot (optional, only needed for plots)
* Experiments in the paper were run on **Windows** with MinGW GCC (exact version: see the paper), Gnuplot 6.0 (patchlevel 5), on an Intel Core i3-1115G4 (2 cores / 4 threads) with 8 GB RAM.
* **Linux:** add `-DO_BINARY=0` to the compile commands (`O_BINARY` exists only on Windows). The live Gnuplot window class `plotter` currently uses Windows-specific calls (`_popen`, `set terminal windows`), so on Linux plot the saved CSV files with Gnuplot directly.

## Build

Run these commands inside the folder that contains the `.cpp` and `.h` files (`ML-Framework/`; change the `-I` path if your layout differs).

```bash
# 1. library objects
g++ -std=c++17 -O2 -fopenmp -I. -c csv.cpp dataset_utils.cpp encoder.cpp linear_regression.cpp \
    matrix.cpp matrix_math.cpp ml_exception.cpp ml_math.cpp random_engine.cpp scaler.cpp

# 2. dataset splitter
g++ -std=c++17 -O2 -fopenmp -I. dataset_splitter.cpp csv.o dataset_utils.o matrix.o \
    matrix_math.o ml_exception.o -o dataset_splitter

# 3. batch gradient descent trainer (writes CSV files)
g++ -std=c++17 -O2 -fopenmp -I. lin_reg_batch_gradient.cpp csv.o matrix.o matrix_math.o \
    ml_exception.o ml_math.o random_engine.o scaler.o -o lin_reg_batch_gradient

# 4. trainer with live Gnuplot windows (Windows only)
g++ -std=c++17 -O2 -fopenmp -I. batch_gradient.cpp plotter.cpp csv.o linear_regression.o \
    matrix.o matrix_math.o ml_exception.o ml_math.o random_engine.o scaler.o -o batch_gradient
```

## Run the example (ice-cream sales)

`IceCreamSales.csv` has a header line, one feature (temperature) and the target (profit).

```bash
# split 70:30, 1 header line; output files are written with the header repeated
./dataset_splitter IceCreamSales.csv 70 1 training_icecream_sales.csv testing_icecream_sales.csv

# train (20,000 cycles, learning rate 0.01) and evaluate on the test file
./lin_reg_batch_gradient
```

The trainer prints the cost every 1,000 cycles and finally `R2Score : ...`. File names are case-sensitive on Linux, so keep the lower-case names shown above.

Files written by the trainer:

| File | Content |
|---|---|
| `icecream_sales_cost.csv` | cycle number, cost J |
| `icecream_sales_model.csv` | learned parameters (bias first) |
| `icecream_sales_min_max.csv` | min / max of every column, from the training data |
| `icecream_sales_scaled_training_data.csv` | scaled feature, target |
| `icecream_sales_line_data.csv` | scaled feature, predicted value |

Plot with Gnuplot, for example:

```
gnuplot> plot "icecream_sales_cost.csv" with lines
gnuplot> plot "icecream_sales_line_data.csv" with lines
```

## Results reported in the paper

Dataset: 365 samples, 70:30 split (255 training / 110 test), learning rate 0.01, 20,000 cycles, parameters initialised from U(0,1).

| | R² | MSE | RMSE |
|---|---|---|---|
| Training | 0.97685 | 6.1555 | 2.4810 |
| Test | 0.97745 | 5.1745 | 2.2748 |

The split and the initial parameters are random (`std::random_device`), so every run gives slightly different numbers; the test R² also depends on which rows land in the test set.

## Known issues

* Copy / move constructors and assignments of `matrix` do not copy the internal `flags` member.
* `matrix::operator*=` wrongly requires the right operand to be square.
* `ml_math::r2score` divides by zero when all target values are equal.
* `OpenMP` regions are started even for very small operands; a size threshold is planned.

## Citation

If you use this code, please cite the paper (details will be added after publication).

## License

Add a `LICENSE` file (for example MIT) before sharing the repository publicly.
