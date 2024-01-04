# Developing a Genetic Algorithm in C

## Overview

This repository contains the implementation and deployment of a Genetic Algorithm entirely from scratch in C. Genetic Algorithms are optimization algorithms inspired by the process of natural selection. They are commonly used for solving complex optimization problems and search problems.

## Introduction

Genetic Algorithms mimic the process of natural selection to evolve solutions to optimization and search problems. The algorithm operates on a population of potential solutions and applies genetic operations such as selection, crossover, and mutation to generate new generations.

This implementation provides a basic framework for developing and experimenting with Genetic Algorithms in the C programming language.

## Using the Genetic Algorithm

1. Clone the repository to your local machine from your terminal:
```
git clone https://github.com/andrewderango/Genetic-Optimizer.git
```
2. Navigate to the project directory:
```
Genetic-Optimizer
```
3. Compile the source code:
```
make
```
4. Execute the executable with your desired parameters:
```
./GA <POPULATION_SIZE> <MAX_GENERATIONS> <crossover_rate> <mutate_rate> <stop_criteria>
```
For example, to run a genetic algorithm on the Ackley function with a population size of 1000, 1000 max generations, a crossover rate of 40% and a mutation rate of 40% and no stop criteria, we can enter the following:
```
./GA 1000 1000 0.4 0.4 0
```
5. Observe the results.\
Running the above parameters gives us the following output:
```
Genetic Algorithm is initiated.
-------------------------------------------------------------
Number of Variables: 2
Lower Bounds: [-5.000000, -5.000000]
Upper Bounds: [5.000000, 5.000000]

Population Size: 1000
Max Generations: 1000
Crossover Rate: 0.400
Mutate Rate: 0.400
Stopping Criteria: 0.000e+00

Results
-------------------------------------------------------------
CPU time: 0.349483 seconds
Best Solution Found: (-0.000000000000000, -0.000000000000000)
Best Fitness: 0.000000000000000
Generations Elapsed: 1000
```
Not bad!

## Customization
The Genetic Algorithm is designed to be flexible and can be applied to various optimization problems. To use the algorithm for a specific problem, you may need to adjust the number of variables, the search range, and the optimization function in ```OF.c```.