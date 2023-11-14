#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "functions.h"

int main(int argc, char *argv[])
{
    // Error handling based on user input
    if (argc != 6) {
        fprintf(stderr, "Error: Invalid argument quantity.\nPlease refer to the proper format: %s <POPULATION SIZE> <MAX GENERATIONS> <crossover rate> <mutate rate> <stop criteria>\n", argv[0]);
        return 1;
    }

    int POPULATION_SIZE, MAX_GENERATIONS;
    double crossover_rate, mutate_rate, stop_criteria;
    int converge_count = 0;

    // Check for proper input types and values
    if (sscanf(argv[1], "%d", &POPULATION_SIZE) != 1 ||
        sscanf(argv[2], "%d", &MAX_GENERATIONS) != 1 ||
        sscanf(argv[3], "%lf", &crossover_rate) != 1 ||
        sscanf(argv[4], "%lf", &mutate_rate) != 1 ||
        sscanf(argv[5], "%lf", &stop_criteria) != 1) {
        printf("Error: Invalid argument types.\nPlease provide <int> <int> <double> <double> <double>\n");
        return 1;
    }
    if (POPULATION_SIZE < 1 || 
        MAX_GENERATIONS < 1 || 
        crossover_rate < 0.0 || 
        crossover_rate > 1.0 || 
        mutate_rate < 0.0 || 
        mutate_rate > 1.0 || 
        stop_criteria < 0.0) {
        printf("Error: Invalid argument values. Please provide values within the following range:\n1 < Population Size\n1 < Max Generations\n0.0 < Crossover Rate < 1.0\n0.0 < Mutate Rate < 1.0\n0.0 < Stop Criteria\n");
        return 1;
    }

    int NUM_VARIABLES = 2; // The number of dimensions, i.e. dimension of the search space
    double Lbound[] = {-5.0, -5.0}; // The lower bounds of variables
    double Ubound[] = {5.0, 5.0}; // The upper bounds of variables
    // Length of Lbound and Ubound arrays should be equal to NUM_VARIABLES

    printf("\nGenetic Algorithm is initiated.\n");
    printf("-------------------------------------------------------------\n");

    // Seeding RNG and starting timer
    srand(time(NULL));
    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();

    // Initial printouts: GA Parameters
    printf("Number of Variables: %d\n", NUM_VARIABLES);
    printf("Lower Bounds: [");
    for(int i = 0; i < NUM_VARIABLES; i++) {
        printf("%f", Lbound[i]);
        if (i < NUM_VARIABLES - 1) {
            printf(", ");
        }
    }
    printf("]\n");
    printf("Upper Bounds: [");
    for(int i = 0; i < NUM_VARIABLES; i++) {
        printf("%f", Ubound[i]);
        if (i < NUM_VARIABLES - 1) {
            printf(", ");
        }
    }
    printf("]\n\n");

    printf("Population Size: %d\n", POPULATION_SIZE);
    printf("Max Generations: %d\n", MAX_GENERATIONS);
    printf("Crossover Rate: %.3f\n", crossover_rate);
    printf("Mutate Rate: %.3f\n", mutate_rate);
    printf("Stopping Criteria: %.3e\n", stop_criteria);

    // Array definitions
    double population[POPULATION_SIZE][NUM_VARIABLES];
    double fitness[POPULATION_SIZE];
    double cumulative_fitness_probs[POPULATION_SIZE];
    double new_population[POPULATION_SIZE][NUM_VARIABLES];
    double min_fitness;
    double new_min_fitness;

    // Create Generation 0
    generate_population(POPULATION_SIZE, NUM_VARIABLES, population, Lbound, Ubound);

    // Iterate through generations until stop criteria or max generations is met
    int generation;
    for (generation = 0; generation < MAX_GENERATIONS; generation++) {

        // Fitness for each individual and cumulative fitness probability calculation
        double fitness_sum = 0.0;
        for (int i = 0; i < POPULATION_SIZE; i++) {
            fitness[i] = Objective_function(NUM_VARIABLES, population[i]);
            cumulative_fitness_probs[i] = 1/(fitness[i]+1e-16);
            fitness_sum += cumulative_fitness_probs[i];
            if (i == 0 || fitness[i] < min_fitness) {
                min_fitness = fitness[i];
            }
        }
        for (int i = 0; i < POPULATION_SIZE; i++) {
            cumulative_fitness_probs[i] = cumulative_fitness_probs[i]/fitness_sum + (i > 0 ? cumulative_fitness_probs[i-1] : 0);
        }

        // Select which individuals' gene sequences move on to the next generation based on fitness
        for (int i = 0; i < POPULATION_SIZE; i++) {
            double random = generate_random(0.0, 1.0);
            int left = 0, right = POPULATION_SIZE - 1; // Binary search
            while (left < right) {
                int mid = left + (right - left) / 2;
                if (random < cumulative_fitness_probs[mid]) {
                    right = mid;
                } else {
                    left = mid + 1;
                }
            }
            for (int k = 0; k < NUM_VARIABLES; k++) {
                new_population[i][k] = population[left][k];
            }
            fitness[i] = Objective_function(NUM_VARIABLES, new_population[i]);
        }
        // Copy the selected individuals back to the population array
        for (int i = 0; i < POPULATION_SIZE; i++) {
            for (int k = 0; k < NUM_VARIABLES; k++) {
                population[i][k] = new_population[i][k];
            }
            fitness[i] = Objective_function(NUM_VARIABLES, population[i]);
        }
        
        // Perform crossover and mutation
        crossover(POPULATION_SIZE, NUM_VARIABLES, fitness, new_population, population, crossover_rate);
        mutate(POPULATION_SIZE, NUM_VARIABLES, new_population, population, Lbound, Ubound, mutate_rate, generation);
        
        // Update population and fitness
        for (int i = 0; i < POPULATION_SIZE; i++) {
            for (int j = 0; j < NUM_VARIABLES; j++) {
                population[i][j] = new_population[i][j];
            }
            double current_fitness = Objective_function(NUM_VARIABLES, new_population[i]);
            if (i == 0 || current_fitness < new_min_fitness) {
                new_min_fitness = current_fitness;
            }
        }

        // Check for stop condition
        if (fabs(new_min_fitness - min_fitness) < stop_criteria) {
            converge_count++;
            if (converge_count == 10) {
                generation++;
                break;
            }
        }
        else {
            converge_count = 0;
        }
    }

    // Finding optimal solution and fitness
    double optimal_solution[NUM_VARIABLES];
    double optimal_fitness;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        double current_fitness = Objective_function(NUM_VARIABLES, new_population[i]);
        if (i == 0 || current_fitness < optimal_fitness) {
            optimal_fitness = current_fitness;
            for (int k = 0; k < NUM_VARIABLES; k++) {
                optimal_solution[k] = new_population[i][k];
            }
        }
    }

    // End timer
    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Print the best solution and fitness
    printf("\nResults\n");
    printf("-------------------------------------------------------------\n");
    printf("CPU time: %f seconds\n", cpu_time_used);
    printf("Best Solution Found: (");
    for (int i = 0; i < NUM_VARIABLES; i++) {
        if (i == 0) {
            printf("%.15f, ", optimal_solution[i]);
        }
        else if (i == NUM_VARIABLES - 1) {
            printf("%.15f)", optimal_solution[i]);
        }
        else {
            printf("%.15f, ", optimal_solution[i]);
        
        }
    }
    printf("\nBest Fitness: %.15f\n", optimal_fitness);
    printf("Generations Elapsed: %d\n", generation);
    return 0;
}