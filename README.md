# Roulette Monte Carlo Simulation & Risk Engine

A high-performance simulation engine written in **C** combined with a **Python** data analytics pipeline.
This project implements a Roulette game with a dedicated **Monte Carlo module** to analyze bankruptcy risk (VaR) and expected returns over 100,000+ simulated sessions.

Designed to demonstrate low-level memory management, binary data persistence, and statistical analysis.

## Key Features

### 1. Monte Carlo Risk Analysis
* **Simulation Engine:** Runs thousands of automated sessions to estimate long-term outcomes without user interaction.
* **Statistical Metrics:** Calculates **Mean Final Balance**, **Bankruptcy Rate**, and **Min/Max** outcomes to model volatility.

### 2. Low-Level C Engineering
* **Binary Persistence:** Uses custom binary file I/O (`fwrite`/`fread`) for high-performance storage of player rankings, bypassing slow text parsing.
* **Memory Management:** Manual handling of structures and pointers for optimized execution.

### 3. Data Analytics Pipeline (Python)
* **Binary Parsing:** The `analiza.py` script uses the `struct` library to parse raw C-structs directly from `ranking.bin`, handling memory alignment and padding.
* **Visualization Dashboard:** Generates a 4-panel Matplotlib dashboard showing score distribution, historical trends, and player performance.

### 4. Quality Assurance
* **Unit Testing:** Comprehensive test suite (`test_obsluga_gry.c`) covering 100% of betting logic (Straight Up, Corners, Columns, Colors) ensuring regression stability.
* **Input Sanitization:** Robust handling of `stdin` to prevent buffer overflows and segmentation faults.

---

## Tech Stack

* **Core Logic:** C (C99 Standard), GCC, Make
* **Analytics:** Python 3.x, Matplotlib, Struct
* **Concepts:** Algorithms, Probability Theory, Regression Testing, Binary I/O

---

## Project Structure

* `main.c`: The main entry point for the interactive game and simulation menu.
* `obsluga_gry.c` / `obsluga_gry.h`: Core game logic, betting mechanics, binary I/O handling, and the Monte Carlo simulation engine.
* `MAKEFILE.h`: Build automation script (Makefile) for compiling the game and test suite.
* `test_obsluga_gry.c`: Unit tests ensuring the mathematical accuracy of payout calculations.
* `analiza.py`: Python script to parse `ranking.bin` and generate data visualizations.
* `ranking.bin`: Automatically generated binary file storing top player scores.

---

## How to Run

### 1. Compilation
The project uses `make` for easy compilation. Open your terminal in the project directory and run:
make

### 2. Running the Game & Simulation
To start :
./ruletka

### 3. Running the Analytics Dashboard (Python)
After playing a few rounds (to generate data in `ranking.bin`), run the Python script to visualize the results:
pip install matplotlib
python analiza.py

### 4. Running the Unit Tests
To verify the core logic and ensure no regressions occurred:
make test

### 5. Cleaning Up
To remove compiled object files and executables:
make clean
