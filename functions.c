#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// delete whatever headers aren't used upon submission

// function prototype
double Objective_function(double decision_vars[], int NUM_VARIABLES);

// implement a function to return a value between min and max
double generate_random(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

// implement the function to return a random integer value
unsigned int generate_int() {
    return rand();
}

// Function to initialize a random population
// randomly initialize for all values in "population[i][j]""
void generate_population(int POPULATION_SIZE, int NUM_VARIABLES, double population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES]) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < NUM_VARIABLES; j++) {
            population[i][j] = generate_random(Lbound[j], Ubound[j]);
        }
    }
}

// Function to compute the objective function for each member of the population
/* compute "fitness[i]"" for each set of decision variables (individual) or each row in "population"
    by calling "Objective_function" */
void compute_objective_function(int POPULATION_SIZE, int NUM_VARIABLES, double population[POPULATION_SIZE][NUM_VARIABLES], double fitness[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness[i] = Objective_function(NUM_VARIABLES, population[i]);
    }
}

/* Implement the logic of crossover function here based on "fitness_probs" or each set
of decision variables (individual) or each row in "population".
And save the new population in "new_population"*/
void crossover(int POPULATION_SIZE, int NUM_VARIABLES, double fitness[POPULATION_SIZE], double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double crossover_rate) {
    for (int i = 0; i < POPULATION_SIZE; i += 2) {

        // printf("i: %d\n", i);

        if (generate_random(0, 1) < crossover_rate) {

            double* parent1 = new_population[i];
            double* parent2 = new_population[i + 1];

            // printf("Parent1: %f, %f\n", parent1[0], parent1[1]);
            // printf("Parent2: %f, %f\n", parent2[0], parent2[1]);

            // Pick a random crossover point (1 for 2 variables)
            int crosspoint = generate_random(1, NUM_VARIABLES);
            // printf("Crosspoint: %d\n", crosspoint);

            double child1[NUM_VARIABLES];
            double child2[NUM_VARIABLES];

            // Splitting these up makes the code work for more variables
            for (int j = 0; j < crosspoint; j++) {
                child1[j] = parent1[j];
                child2[j] = parent2[j];
            }

            for (int j = crosspoint; j < NUM_VARIABLES; j++) {
                child1[j] = parent2[j];
                child2[j] = parent1[j];
            }

            // printf("Child1: %f, %f\n", child1[0], child1[1]);
            // printf("Child2: %f, %f\n", child2[0], child2[1]);

            // Replace the parents with the new children
            for (int j = 0; j < NUM_VARIABLES; j++) {
                new_population[i][j] = child1[j];
                new_population[i + 1][j] = child2[j];
            }
        } 
        // else {
        //     printf("No crossover\n");
        // }
        // printf("\n");
    }
}

void mutate(int POPULATION_SIZE, int NUM_VARIABLES, double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES], double mutate_rate) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < NUM_VARIABLES; j++) {
            if (generate_random(0, 1) < mutate_rate) {
                new_population[i][j] = generate_random(Lbound[j], Ubound[j]);
            }
        }
    }

    // copy everything from new_population into population
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < NUM_VARIABLES; j++) {
            population[i][j] = new_population[i][j];
        }
    }
}