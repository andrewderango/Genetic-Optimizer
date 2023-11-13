#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
// delete whatever headers aren't used upon submission

// function prototype
double Objective_function(int NUM_VARIABLES, double x[NUM_VARIABLES]);

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

// Old crossover function
// void crossover(int POPULATION_SIZE, int NUM_VARIABLES, double fitness[POPULATION_SIZE], double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double crossover_rate) {
//     for (int i = 0; i < POPULATION_SIZE; i += 2) {

//         // printf("i: %d\n", i);

//         if (generate_random(0, 1) < crossover_rate) {

//             double* parent1 = new_population[i];
//             double* parent2 = new_population[i + 1];

//             // printf("Parent1: %f, %f\n", parent1[0], parent1[1]);
//             // printf("Parent2: %f, %f\n", parent2[0], parent2[1]);

//             // Pick a random crossover point (1 for 2 variables)
//             int crosspoint = generate_random(1, NUM_VARIABLES);
//             // printf("Crosspoint: %d\n", crosspoint);

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

//             // printf("Child1: %f, %f\n", child1[0], child1[1]);
//             // printf("Child2: %f, %f\n", child2[0], child2[1]);

//             // Replace the parents with the new children
//             for (int j = 0; j < NUM_VARIABLES; j++) {
//                 new_population[i][j] = child1[j];
//                 new_population[i + 1][j] = child2[j];
//             }
//         } 
//         // else {
//         //     printf("No crossover\n");
//         // }
//         // printf("\n");
//     }
// }

// Bonus point; improved crossover function
void crossover(int POPULATION_SIZE, int NUM_VARIABLES, double fitness[POPULATION_SIZE], double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double crossover_rate) {

    double fitness_max;
    double fitness_min;
    double fitness_sum = 0;
    // printf("x1\t\tx2\t\tFitness Score\n");
    for (int i = 0; i < POPULATION_SIZE; i++) {
        // for (int j = 0; j < NUM_VARIABLES; j++) {
        //     printf("%f\t", new_population[i][j]);
        // }
        double point_fitness = fitness[i];
        fitness_sum += point_fitness;
        // printf("%f\n", point_fitness);
        if (i == 0 || point_fitness > fitness_max) {
            fitness_max = point_fitness;
        }
        if (i == 0 || point_fitness < fitness_min) {
            fitness_min = point_fitness;
        }
    }
    double fitness_avg = fitness_sum/POPULATION_SIZE;
    // printf("Fitness Max: %f\n", fitness_max);
    // printf("Fitness Min: %f\n", fitness_min);
    // printf("Fitness Sum: %f\n", fitness_sum);
    // printf("Fitness Average: %f\n", fitness_avg);

    double squared_diff_sum = 0.0;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        double diff = fitness[i] - (fitness_sum/POPULATION_SIZE);
        squared_diff_sum += diff * diff;
    }
    double fitness_stdev = sqrt(squared_diff_sum / POPULATION_SIZE);

    // printf("Standard Deviation: %f\n", fitness_stdev);
    double exp_stdev_sum = 0.0;
    for (int i = 0; i < POPULATION_SIZE; i++) {
        exp_stdev_sum += exp(-(fitness[i]-fitness_avg)/(fitness_stdev+1e-16));
        // printf("%f ", exp(-(fitness[i]-fitness_avg)/fitness_stdev)/fitness_stdev);
    }
    // printf("\n");
    double cumulative_exp_stdev_sum = 0.0;
    double crossover_index_prob[POPULATION_SIZE];
    for (int i = 0; i < POPULATION_SIZE; i++) {
        crossover_index_prob[i] = exp(-(fitness[i]-fitness_avg)/(fitness_stdev+1e-16))/(exp_stdev_sum+1e-16) + cumulative_exp_stdev_sum;
        // printf("%f ", exp(-(fitness[i]-fitness_avg)/fitness_stdev)/exp_stdev_sum + cumulative_exp_stdev_sum);
        cumulative_exp_stdev_sum += exp(-(fitness[i]-fitness_avg)/(fitness_stdev+1e-16))/(exp_stdev_sum+1e-16);
    }
    // printf("\n");

    for (int i = 0; i < POPULATION_SIZE; i += 2) {

        // printf("i: %d\n", i);

        if (generate_random(0, 1) < crossover_rate) {

            double parent1[NUM_VARIABLES];
            double parent2[NUM_VARIABLES];
            double parent_1_seed = generate_random(0, 1);
            double parent_2_seed = generate_random(0, 1);

            // printf("Parent 1 Seed: %f\n", parent_1_seed);
            // printf("Parent 2 Seed: %f\n", parent_2_seed);

            for (int j = 0; j < POPULATION_SIZE; j++) {
                if (parent_1_seed < crossover_index_prob[j]) {
                    for (int k = 0; k < NUM_VARIABLES; k++) {
                        parent1[k] = new_population[j][k];
                    }
                    break;
                }
            }
            for (int j = 0; j < POPULATION_SIZE; j++) {
                if (parent_2_seed < crossover_index_prob[j]) {
                    for (int k = 0; k < NUM_VARIABLES; k++) {
                        parent2[k] = new_population[j][k];
                    }
                    break;
                }
            }

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

    // printf("\n-- CROSSOVER --\n");
    // for (int j = 0; j < POPULATION_SIZE; j++) {
    //     for (int k = 0; k < NUM_VARIABLES; k++) {
    //         printf("%f\t", new_population[j][k]);
    //     }
    //     printf("%f\n", Objective_function(NUM_VARIABLES, new_population[j]));
    // }
}

// Old mutate function
// void mutate(int POPULATION_SIZE, int NUM_VARIABLES, double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES], double mutate_rate) {

//     int target_mutation = POPULATION_SIZE * NUM_VARIABLES * mutate_rate;

//     // printf("Population Size: %d\n", POPULATION_SIZE);
//     // printf("Number of Variables: %d\n", NUM_VARIABLES);
//     // printf("Mutate Rate: %f\n", mutate_rate);
//     // printf("Genes to mutate: %d\n\n", target_mutation);

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

//     // printf("Mutate indexes: ");
//     // for (int i = 0; i < target_mutation; i++) {
//     //     printf("%d ", mutate_indexes[i]);
//     // }
//     // printf("\n");

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

// Improved mutate function; bonus
void mutate(int POPULATION_SIZE, int NUM_VARIABLES, double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES], double mutate_rate, int generation) {

    double best_fitness_coord[NUM_VARIABLES];
    double best_fitness;

    int target_mutation = POPULATION_SIZE * NUM_VARIABLES * mutate_rate;

    // Definition of the arbitrary activator function
    double HZ_TRANSLATION = -1.09219;
    double DECAY_INTENSITY = -16.1197;
    double activator = exp((generation+HZ_TRANSLATION)/DECAY_INTENSITY);

    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < NUM_VARIABLES; j++) {
            double point_fitness = Objective_function(NUM_VARIABLES, new_population[i]);
            if (i == 0 || point_fitness < best_fitness) {
                best_fitness = point_fitness;
                for (int k = 0; k < NUM_VARIABLES; k++) {
                    best_fitness_coord[k] = new_population[i][k];
                }
            }
        }
    }

    printf("Best Fitness: %f\n", best_fitness);
    printf("Best Fitness Coordinates: ");
    for (int i = 0; i < NUM_VARIABLES; i++) {
        printf("%f ", best_fitness_coord[i]);
    }
    printf("\n");

    // printf("Generation: %d\n", generation);
    printf("Activator: %f\n", activator);
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

    for (int i = 0; i < NUM_VARIABLES; i++) {
        printf("Lbound[%d]: %f\n", i, Lbound[i]);
        printf("Ubound[%d]: %f\n", i, Ubound[i]);
    }

    int mutate_index;
    int mutate_indexes[target_mutation];

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

    // printf("Mutate indexes: ");
    // for (int i = 0; i < target_mutation; i++) {
    //     printf("%d ", mutate_indexes[i]);
    // }
    // printf("\n");


    for (int i = 0; i < POPULATION_SIZE; i++) {
        for (int j = 0; j < NUM_VARIABLES; j++) {
            for (int k = 0; k < target_mutation; k++) {
                if (i*NUM_VARIABLES + j == mutate_indexes[k]) {
                    new_population[i][j] = generate_random(Lbound[j], Ubound[j]);
                }
            }
        }
    }
}