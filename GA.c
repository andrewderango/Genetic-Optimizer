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

    // Check for proper input types
    if (sscanf(argv[1], "%d", &POPULATION_SIZE) != 1 ||
        sscanf(argv[2], "%d", &MAX_GENERATIONS) != 1 ||
        sscanf(argv[3], "%lf", &crossover_rate) != 1 ||
        sscanf(argv[4], "%lf", &mutate_rate) != 1 ||
        sscanf(argv[5], "%lf", &stop_criteria) != 1) {
        fprintf(stderr, "Error: Invalid argument types.\nPlease provide <int> <int> <double> <double> <double>\n");
        return 1;
    }

    int NUM_VARIABLES = 2; // the number of variables
    double Lbound[] = {-5.0, -5.0}; // the lower bounds of variables
    double Ubound[] = {5.0, 5.0}; // the upper bounds of variable
    // Length of Lbound and Ubound should be equal to NUM_VARIABLES

    printf("Genetic Algorithm is initiated.\n");
    printf("-----------------------------------------\n");

    // Seeding RNG and starting timer
    srand(time(NULL));
    clock_t start_time, end_time;
    double cpu_time_used;
    start_time = clock();

    // Initial printouts: GA Parameters
    printf("Number of Variables: %d\n", NUM_VARIABLES);
    printf("Lower Bounds: [%f, %f]\n", Lbound[0], Lbound[1]); // change this for more vars
    printf("Upper Bounds: [%f, %f]\n\n", Ubound[0], Ubound[1]); // change this for more vars
    printf("Population Size: %d\n", POPULATION_SIZE);
    printf("Max Generations: %d\n", MAX_GENERATIONS);
    printf("Crossover Rate: %f\n", crossover_rate);
    printf("Mutate Rate: %f\n", mutate_rate);
    printf("Stopping Criteria: %f\n", stop_criteria);

    // Array definitions
    double population[POPULATION_SIZE][NUM_VARIABLES];
    double fitness[POPULATION_SIZE];
    double cumulative_fitness_probs[POPULATION_SIZE];
    double new_population[POPULATION_SIZE][NUM_VARIABLES];
    double min_fitness;
    double new_min_fitness;

    // Create Generation 0
    generate_population(POPULATION_SIZE, NUM_VARIABLES, population, Lbound, Ubound);

    // iteration starts here. The loop continues until MAX_GENERATIONS is reached
    // Or stopping criteria is met
    int generation;
    for (generation = 0; generation < MAX_GENERATIONS; generation++) {
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

        // printf("-- INITIAL POPULATION --\n");
        // printf("x1\t\tx2\t\tFitness\t\tCumulative Scaled Fitness\n");
        // for (int i = 0; i < POPULATION_SIZE; i++) {
        //     for (int j = 0; j < NUM_VARIABLES; j++) {
        //         printf("%f\t", population[i][j]);
        //     }
        //     printf("%f\t%f\n", fitness[i], cumulative_fitness_probs[i]);
        // }
        // printf("Best Fitness: %f\n", min_fitness);

        // <YOUR CODE: Here implement the logic of finding best solution with minimum fitness value
        // and the stopping criteria>

        // printf("\n-- EVOLUTION --\n");
        // printf("Random\t\tIndex\tx1\t\tx2\n");
        for (int i = 0; i < POPULATION_SIZE; i++) {
            double random = generate_random(0.0, 1.0);
            // printf("%f\t", random);
            for (int j = 0; j < POPULATION_SIZE; j++) {
                if (random < cumulative_fitness_probs[j]) {
                    // printf("%d\t", j);
                    for (int k = 0; k < NUM_VARIABLES; k++) {
                        new_population[i][k] = population[j][k];
                        fitness[i] = Objective_function(NUM_VARIABLES, new_population[i]);
                        // printf("%f\t", new_population[i][k]);
                    }
                    // printf("\n");
                    break;
                }
            }
        }

        // <YOUR CODE: Here call the crossover function>
        
        // printf("\n-- CROSSOVER --\n");
        crossover(POPULATION_SIZE, NUM_VARIABLES, fitness, new_population, population, crossover_rate);
        // for (int i = 0; i < POPULATION_SIZE; i++) {
        //     for (int j = 0; j < NUM_VARIABLES; j++) {
        //         printf("%f ", new_population[i][j]);
        //     }
        //     printf("\n");
        // }

        // <YOUR CODE: Here call the mutation function>

        // printf("\n-- MUTATION --\n");
        // printf("x1\t\tx2\t\tFitness\n");

        population[0][0] = generation; // I know this is a bad way to do this. But I can't edit the header file, so this is the only way to access the generation number in the mutate function.
        mutate(POPULATION_SIZE, NUM_VARIABLES, new_population, population, Lbound, Ubound, mutate_rate);
        
        for (int i = 0; i < POPULATION_SIZE; i++) {
            for (int j = 0; j < NUM_VARIABLES; j++) {
                population[i][j] = new_population[i][j];
                // printf("%f\t", new_population[i][j]);
            }
            // printf("%f\n", Objective_function(NUM_VARIABLES, new_population[i]));
            if (i == 0 || Objective_function(NUM_VARIABLES, new_population[i]) < new_min_fitness) {
                new_min_fitness = Objective_function(NUM_VARIABLES, new_population[i]);
            }
        }
        // printf("Best Fitness: %f\n\n", new_min_fitness);

        // Now you have the a new population, and it goes to the beginning of loop to re-compute all again

        // printf("Generation %d Progress\n", generation + 1);
        printf("Previous best fitness: %f\n", min_fitness);
        printf("New best fitness: %f\n", new_min_fitness);
        if (fabs(new_min_fitness - min_fitness) < stop_criteria) {
            converge_count++;
            if (converge_count == 10) {
                generation++;
                // printf("Converged at Generation %d\n", generation);
                break;
            }
        }
        else {
            converge_count = 0;
        }
        printf("Converge count: %d\n", converge_count);
    }

    // Find best soluton by minimum fitness
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

    end_time = clock();
    cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Print the best solution and fitness
    printf("\nResults\n");
    printf("-----------------------------------------\n");
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
    printf("Generations Used: %d\n", generation);
    return 0;
}