#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Objective function prototype
double Objective_function(int NUM_VARIABLES, double x[NUM_VARIABLES]);

// Function to return random double value between min and max
double generate_random(double min, double max) {
    return min + ((double)rand() / RAND_MAX) * (max - min);
}

// Function to return random int value
unsigned int generate_int() {
    return rand();
}

// Function to initialize a random population
void generate_population(int POPULATION_SIZE, int NUM_VARIABLES, double population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES]) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < NUM_VARIABLES; j++) {
            population[i][j] = generate_random(Lbound[j], Ubound[j]);
        }
    }
}

// Function to compute the objective function for each member of the population
void compute_objective_function(int POPULATION_SIZE, int NUM_VARIABLES, double population[POPULATION_SIZE][NUM_VARIABLES], double fitness[POPULATION_SIZE]) {
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness[i] = Objective_function(NUM_VARIABLES, population[i]);
    }
}

// Improved crossover function; bonus
void crossover(int POPULATION_SIZE, int NUM_VARIABLES, double fitness[POPULATION_SIZE], double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double crossover_rate) {

    // Calculate total fitness
    double fitness_sum = 0;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        if (fitness[i] != 0) {
            fitness_sum += 1 / fitness[i];
        }
    }

    // Calculate the cumulative probability of each individual
    double cumulative_prob_sum = 0.0;
    double crossover_index_prob[POPULATION_SIZE];
    for (int i = 0; i < POPULATION_SIZE; i++) {
        if (fitness[i] != 0) {
            double prob = (1 / fitness[i]) / fitness_sum;
            crossover_index_prob[i] = prob + cumulative_prob_sum;
            cumulative_prob_sum += prob;
        } else {
            crossover_index_prob[i] = 0;
        }
    }

    // Iterate through the population and perform crossover
    for (int i = 0; i < POPULATION_SIZE; i += 2) {
        if (generate_random(0, 1) < crossover_rate) {
            int parent1_index, parent2_index;

            // Binary search for parent 1
            int start = 0, end = POPULATION_SIZE - 1;
            double parent_1_seed = generate_random(0, 1);
            while (start <= end) {
                int mid = start + (end - start) / 2;
                if (parent_1_seed < crossover_index_prob[mid]) {
                    end = mid - 1;
                } else {
                    start = mid + 1;
                }
            }
            parent1_index = start;

            // Binary search for parent 2
            start = 0, end = POPULATION_SIZE - 1;
            double parent_2_seed = generate_random(0, 1);
            while (start <= end) {
                int mid = start + (end - start) / 2;
                if (parent_2_seed < crossover_index_prob[mid]) {
                    end = mid - 1;
                } else {
                    start = mid + 1;
                }
            }
            parent2_index = start;

            // Pick a random crossover point (always 1 for 2 variables)
            int crosspoint = generate_random(1, NUM_VARIABLES);

            // Perform crossover and replace the parents with the new children
            for (int j = 0; j < crosspoint; j++) {
                new_population[i][j] = population[parent1_index][j];
                new_population[i + 1][j] = population[parent2_index][j];
            }
            for (int j = crosspoint; j < NUM_VARIABLES; j++) {
                new_population[i][j] = population[parent2_index][j];
                new_population[i + 1][j] = population[parent1_index][j];
            }
        } 
    }
}

// Improved mutate function; bonus
void mutate(int POPULATION_SIZE, int NUM_VARIABLES, double new_population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES], double mutate_rate, int generation) {

    double best_fitness_coord[NUM_VARIABLES];
    double best_fitness;
    double fitness[POPULATION_SIZE];
    for (int i = 0; i < POPULATION_SIZE; i++) {
        fitness[i] = Objective_function(NUM_VARIABLES, new_population[i]);
    }
    int target_mutation = POPULATION_SIZE * NUM_VARIABLES * mutate_rate; // Number of genes to mutate

    // Definition of the arbitrary activator function
    double HZ_TRANSLATION = -1.09219;
    double DECAY_INTENSITY = -16.1197;
    double activator = exp((generation+HZ_TRANSLATION)/DECAY_INTENSITY);

    // Find the best fitness
    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < NUM_VARIABLES; j++) {
            double point_fitness = fitness[i];
            if (i == 0 || point_fitness < best_fitness) {
                best_fitness = point_fitness;
                for (int k = 0; k < NUM_VARIABLES; k++) {
                    best_fitness_coord[k] = new_population[i][k];
                }
            }
        }
    }

    // Adjust search bounds
    double range[NUM_VARIABLES];
    for (int i = 0; i < NUM_VARIABLES; i++) {
        range[i] = Ubound[i] - Lbound[i];
    }
    for (int i = 0; i < NUM_VARIABLES; i++) {
        if (best_fitness_coord[i] - range[i]*activator/2 > Lbound[i]) {
            Lbound[i] = best_fitness_coord[i] - range[i]*activator/2;
        } 
        if (best_fitness_coord[i] + range[i]*activator/2 < Ubound[i]) {
            Ubound[i] = best_fitness_coord[i] + range[i]*activator/2;
        }
    }

    int mutate_index;
    int mutate_indexes[target_mutation];

    // Set mutation indexes
    for (int i = 0; i < target_mutation; i++) {
        mutate_index = generate_int() % (POPULATION_SIZE * NUM_VARIABLES);
        for (int j = 0; j < i; j++) {
            if (mutate_index == mutate_indexes[j]) {
                mutate_index = generate_int() % (POPULATION_SIZE * NUM_VARIABLES);
                j = 0;
            }
        }
        mutate_indexes[i] = mutate_index;
    }

    // Perform mutations
    for (int i = 0; i < target_mutation; i++) {
        int individual_index = mutate_indexes[i] / NUM_VARIABLES;
        int variable_index = mutate_indexes[i] % NUM_VARIABLES;
        new_population[individual_index][variable_index] = generate_random(Lbound[variable_index], Ubound[variable_index]);
    }
}

// ORIGINAL CROSSOVER AND MUTATE FUNCTIONS (NO IMPROVEMENTS) ARE COMMENTED BELOW

// void crossover(int POPULATION_SIZE, int NUM_VARIABLES, double fitness[POPULATION_SIZE], double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double crossover_rate) {
//     for (int i = 0; i < POPULATION_SIZE; i += 2) {

//         if (generate_random(0, 1) < crossover_rate) {

//             double* parent1 = population[i];
//             double* parent2 = population[i + 1];

//             // Pick a random crossover point (1 for 2 variables)
//             int crosspoint = generate_random(1, NUM_VARIABLES);

//             double child1[NUM_VARIABLES];
//             double child2[NUM_VARIABLES];

//             // Splitting these up makes the code work for more variables
//             for (int j = 0; j < crosspoint; j++) {
//                 child1[j] = parent1[j];
//                 child2[j] = parent2[j];
//             }

//             for (int j = crosspoint; j < NUM_VARIABLES; j++) {
//                 child1[j] = parent2[j];
//                 child2[j] = parent1[j];
//             }

//             // Replace the parents with the new children
//             for (int j = 0; j < NUM_VARIABLES; j++) {
//                 new_population[i][j] = child1[j];
//                 new_population[i + 1][j] = child2[j];
//             }
//         }
//     }
// }

// void mutate(int POPULATION_SIZE, int NUM_VARIABLES, double new_population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES], double mutate_rate, int generations) {

//     int target_mutation = POPULATION_SIZE * NUM_VARIABLES * mutate_rate; // Number of genes to mutate
//     int mutate_index;
//     int mutate_indexes[target_mutation];

//     for (int i = 0; i < target_mutation; i++) {
//         mutate_index = generate_int() % (POPULATION_SIZE * NUM_VARIABLES);
//         for (int j = 0; j < i; j++) {
//             if (mutate_index == mutate_indexes[j]) {
//                 mutate_index = generate_int() % (POPULATION_SIZE * NUM_VARIABLES);
//                 j = 0;
//             }
//         }
//         mutate_indexes[i] = mutate_index;
//     }

//     for (int i = 0; i < POPULATION_SIZE; i++) {
//         for (int j = 0; j < NUM_VARIABLES; j++) {
//             for (int k = 0; k < target_mutation; k++) {
//                 if (i*NUM_VARIABLES + j == mutate_indexes[k]) {
//                     new_population[i][j] = generate_random(Lbound[j], Ubound[j]);
//                 }
//             }
//         }
//     }
// }