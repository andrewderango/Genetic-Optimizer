#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "functions.h"

int main(int argc, char *argv[])
{
    // <YOUR CODE: Handle the possible errors in input data given by the user and say how to execute the code>

    if (argc != 6) {
        fprintf(stderr, "Usage: %s <POPULATION SIZE> <MAX GENERATIONS> <crossover rate> <mutate rate> <stop criteria>\n", argv[0]);
        return 1;
    }

    // <YOUR CODE: Assign all inputs given by the user argv[i]> like
    // POPULATION_SIZE, MAX_GENERATIONS, crossover_rate, mutate_rate, stop_criteria

    int POPULATION_SIZE = atoi(argv[1]);
    int MAX_GENERATIONS = atoi(argv[2]);
    double crossover_rate = atof(argv[3]);
    double mutate_rate = atof(argv[4]);
    double stop_criteria = atof(argv[5]);

    // ###################################################################################
    // you dont need to change anything here
    // the number of variables
    int NUM_VARIABLES = 2;
    // the lower bounds of variables
    double Lbound[] = {-5.0, -5.0};
    // the upper bounds of variable
    double Ubound[] = {5.0, 5.0};
    // ###################################################################################

    // <YOUR CODE: Here make all the initial print outs>

    srand(time(NULL));
    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();

    printf("-- EVOLUTION PARAMETERS --\n");
    printf("Population size: %d\n", POPULATION_SIZE);
    printf("Max generations: %d\n", MAX_GENERATIONS);
    printf("Crossover rate: %f\n", crossover_rate);
    printf("Mutate rate: %f\n", mutate_rate);
    printf("Stop criteria: %f\n", stop_criteria);

    // <YOUR CODE: Declare all the arrays you need here>

    double population[POPULATION_SIZE][NUM_VARIABLES];
    double fitness[POPULATION_SIZE];
    double cumulative_fitness_probs[POPULATION_SIZE];
    double new_population[POPULATION_SIZE][NUM_VARIABLES];
    double min_fitness;
    double new_min_fitness;

    // <YOUR CODE: Call generate_population function to initialize the "population"> like:
    // generate_population(POPULATION_SIZE, NUM_VARIABLES, population, Lbound, Ubound);

    generate_population(POPULATION_SIZE, NUM_VARIABLES, population, Lbound, Ubound);

    // iteration starts here. The loop continues until MAX_GENERATIONS is reached
    // Or stopping criteria is met
    for (int generation = 0; generation < MAX_GENERATIONS; generation++) {
        printf("\n-- GENERATION %d --\n", generation + 1);

        // <YOUR CODE: Compute the fitness values using objective function for
        // each row in "population" (each set of variables)> like:
        // compute_objective_function(POPULATION_SIZE, NUM_VARIABLES, population, fitness);

        for (int i = 0; i < POPULATION_SIZE; i++) {
            fitness[i] = Objective_function(NUM_VARIABLES, population[i]);
            cumulative_fitness_probs[i] = 1/(fitness[i]+1e-6);
            if (i == 0 || fitness[i] < min_fitness) {
                min_fitness = fitness[i];
            }
        }

        double fitness_sum = 0.0;
        for (int i = 0; i < POPULATION_SIZE; i++) {
            fitness_sum += cumulative_fitness_probs[i];
        }
        for (int i = 0; i < POPULATION_SIZE; i++) {
            cumulative_fitness_probs[i] = cumulative_fitness_probs[i]/fitness_sum + (i > 0 ? cumulative_fitness_probs[i-1] : 0);
        }

        printf("-- INITIAL POPULATION --\n");
        printf("x1\t\tx2\t\tFitness\t\tCumulative Scaled Fitness\n");
        for (int i = 0; i < POPULATION_SIZE; i++) {
            for (int j = 0; j < NUM_VARIABLES; j++) {
                printf("%f\t", population[i][j]);
            }
            printf("%f\t%f\n", fitness[i], cumulative_fitness_probs[i]);
        }
        printf("Best Fitness: %f\n", min_fitness);

        // <YOUR CODE: Here implement the logic of finding best solution with minimum fitness value
        // and the stopping criteria>

        printf("\n-- EVOLUTION --\n");
        printf("Random\t\tIndex\tx1\t\tx2\n");
        for (int i = 0; i < POPULATION_SIZE; i++) {
            double random = generate_random(0.0, 1.0);
            printf("%f\t", random);
            for (int j = 0; j < POPULATION_SIZE; j++) {
                if (random < cumulative_fitness_probs[j]) {
                    printf("%d\t", j);
                    for (int k = 0; k < NUM_VARIABLES; k++) {
                        new_population[i][k] = population[j][k];
                        printf("%f\t", new_population[i][k]);
                    }
                    printf("\n");
                    break;
                }
            }
        }

        // <YOUR CODE: Here call the crossover function>
        
        printf("\n-- CROSSOVER --\n");
        crossover(POPULATION_SIZE, NUM_VARIABLES, fitness, new_population, population, crossover_rate);
        for (int i = 0; i < POPULATION_SIZE; i++) {
            for (int j = 0; j < NUM_VARIABLES; j++) {
                printf("%f ", new_population[i][j]);
            }
            printf("\n");
        }

        // <YOUR CODE: Here call the mutation function>

        printf("\n-- MUTATION --\n");
        printf("x1\t\tx2\t\tFitness\n");
        mutate(POPULATION_SIZE, NUM_VARIABLES, new_population, population, Lbound, Ubound, mutate_rate);
        for (int i = 0; i < POPULATION_SIZE; i++) {
            for (int j = 0; j < NUM_VARIABLES; j++) {
                population[i][j] = new_population[i][j];
                printf("%f\t", new_population[i][j]);
            }
            printf("%f\n", Objective_function(NUM_VARIABLES, new_population[i]));
            if (i == 0 || Objective_function(NUM_VARIABLES, new_population[i]) < new_min_fitness) {
                new_min_fitness = Objective_function(NUM_VARIABLES, new_population[i]);
            }
        }
        printf("Best Fitness: %f\n\n", new_min_fitness);

        // Now you have the a new population, and it goes to the beginning of loop to re-compute all again

        printf("Generation %d Progress\n", generation + 1);
        printf("Previous best fitness: %f\n", min_fitness);
        printf("New best fitness: %f\n", new_min_fitness);
        if (fabs(new_min_fitness - min_fitness) < stop_criteria) {
            break;
        }
    }

    // <YOUR CODE: Jump to this part of code if the stopping criteria is met before MAX_GENERATIONS is met>

    double optimal_solution[NUM_VARIABLES];
    double optimal_fitness;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < NUM_VARIABLES; j++) {
            if (i == 0 || Objective_function(NUM_VARIABLES, new_population[i]) < optimal_fitness) {
                optimal_fitness = Objective_function(NUM_VARIABLES, new_population[i]);
                for (int k = 0; k < NUM_VARIABLES; k++) {
                    optimal_solution[k] = new_population[i][k];
                }
            }
        }
    }

    // ###################################################################################
    // You dont need to change anything here
    // Here we print the CPU time taken for your code
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    printf("\n-- PROGRAM PERFORMANCE --\n");
    printf("CPU time: %f seconds\n", cpu_time_used);
    // ###################################################################################

    // <Here print out the best solution and objective function value for the best solution like the format>

    printf("\n-- FINAL SOLUTION --\n");
    for (int i = 0; i < NUM_VARIABLES; i++) {
        printf("x%d: %f\n", i + 1, optimal_solution[i]);
    }
    printf("Optimal Fitness: %f\n", optimal_fitness);

    return 0;
}