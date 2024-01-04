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
If we plot the 3D Ackley function in the specified range, we see that the absolute minimum is precisely at (0, 0) with a fitness of 0.

<p align="center">
  <img src="https://github.com/andrewderango/Genetic-Optimizer/assets/93727693/17c5abd7-250c-4a28-9b58-336881df6bbd" alt="3D Ackley Function via Desmos" width="500">
</p>

Not bad!

## Customization
The Genetic Algorithm is designed to be flexible and can be applied to various optimization problems. To use the algorithm for a specific problem, you may need to adjust the number of variables, the search range, and the optimization function in ```OF.c```. The default optimization function is the [Ackley function](https://en.wikipedia.org/wiki/Ackley_function) in ```n``` dimensions, where ```n``` is the number of variables specified by the user.

## Improving the Performance
Several changes were implemented to the standard Genetic Algorithm procedure in order to either accelerate the evolution process or find a more accurate minimum.

### Mutation Function Enhancement
The primary and pivotal modification lies in the mutation function. Departing from the conventional approach of random mutations within a specified range, which often yields a minimal likelihood of rendering a strong solution, a strategic adjustment has been implemented. The mutation process is now generation-dependent, causing the mutation range to gradually converge around the point of optimal fitness as generations progress. This approach is still effective for functions whose global minimum isn’t (0, 0, ...) because the early generations are likely to find the hole of the global minimum, and then the search range slowly converges around that point to permit extreme accuracy. For the first generations, the mutation range is still nearly as defined, which effectively maintains genetic diversity when it's most important. The technique has been rigorously validated across various dimensions and functions, including Ackley, Rastrigin, Beale, Levi N. 13, and Himmelblau, consistently yielding results closely aligned with true global minima.

### Adaptive Crossover Function
In the crossover function, a crucial adaptation was made to prioritize the crossing of genes from more fit individuals over their less fit counterparts. Probabilities are determined based on the normalized exponential values of their z-scores relative to the population. This targeted approach strengthens crossover for highly fit individuals while diminishing its impact on weaker counterparts, in contrast to the simpler normalized 1/fitness used in conventional intergenerational individual determination.

### Maintaining Diversity
While these modifications may decrease genetic diversity in the population, this effect is intentionally deferred to later generations. The adjustments are strategically timed to ensure that the global minimum is identified prior to convergence. It's important to note that exceptionally low population counts or extensive initial search ranges may present challenges, but these are common difficulties encountered by standard genetic algorithms as well.

### Evaluating the Improvements
To gauge the impact of the introduced modifications on the algorithm's performance, we shall conduct a comparative analysis of its behaviour before and after the implementation of these changes. For both test cases below, we are using a crossover rate of 50%, a mutation rate of 20%, and a 3D Ackley function.

#### Performance of Standard Genetic Algorithm

| Pop Size | Max Gen | $x_1$                | $x_2$                 | Fitness              | CPU Time (s) |
|:--------:|:-------:|:--------------------:|:---------------------:|:--------------------:|:--------------:|
| 10       | 100     | -0.011278062598444   | 0.019696436365925    | 0.077864075069045    | 0.000080       |
| 100      | 100     | 0.043074449078681    | 0.020379226198597    | 0.194258202498812    | 0.001218       |
| 1000     | 100     | -0.000641841907353   | -0.000256060157091   | 0.001967254744301    | 0.037911       |
| 10000    | 100     | 0.000013338867582    | 0.000035285484062    | 0.000106733375379    | 0.910979       |
| 1000     | 1000    | 0.001356226858383    | 0.002428398003070    | 0.008073129104510    | 0.028995       |
| 1000     | 10000   | 0.000922891777438    | -0.001703875605810   | 0.005580803229666    | 0.029390       |
| 1000     | 100000  | -0.001054171007617   | 0.000625157263421    | 0.003506521480777    | 0.025726       |
| 1000     | 1000000 | -0.000289760064469   | -0.000070461537722   | 0.000845816690682    | 0.043721       |

#### Performance of Enhanced Genetic Algorithm

| Pop Size | Max Gen | $x_1$                   | $x_2$                    | Fitness                | CPU Time (Sec) |
|:--------:|:-------:|:-----------------------:|:------------------------:|:-----------------------:|:--------------:|
|   100    |   100   | 0.000000064633813       | -0.000000348628304       | 0.000001002876119       |     0.004836   |
|  1000    |   100   | 0.000000000000000       | -0.000000000000000       | 0.000000000000000       |     0.023476   |
| 10000    |   100   | 0.000000000000000       | -0.000000000000000       | 0.000000000000000       |     0.285449   |
|  1000    |  1000   | 0.000000000000000       | 0.000000000000000        | 0.000000000000000       |     0.022813   |
|  1000    | 10000   | 0.000000000000000       | 0.000000000000000        | 0.000000000000000       |     0.022514   |
|  1000    | 100000  | -0.000000000000000      | 0.000000000000000        | 0.000000000000000       |     0.023436   |

The enhanced algorithm clearly delivers stronger results.
