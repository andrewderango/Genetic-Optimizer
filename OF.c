#include <math.h>
#include "functions.h"
// Do not change anything in this file. You will not submit this file.

// Ackley function
// Define the objective function function for optimization
double Objective_function(int NUM_VARIABLES, double x[NUM_VARIABLES])
{
    // While the objective function could be anything here it is to minimize Ackley function
    double sum1 = 0.0, sum2 = 0.0;
    for (int i = 0; i < NUM_VARIABLES; i++)
    {
        sum1 += x[i] * x[i];
        sum2 += cos(2.0 * M_PI * x[i]);
    }
    return -20.0 * exp(-0.2 * sqrt(sum1 / NUM_VARIABLES)) - exp(sum2 / NUM_VARIABLES) + 20.0 + M_E;
}

// Rastrigin function. Set Lbound = -5.12 and Ubound = 5.12
// double Objective_function(int NUM_VARIABLES, double x[NUM_VARIABLES])
// {
//     double sum = 0.0;
//     for (int i = 0; i < NUM_VARIABLES; i++)
//     {
//         sum += x[i] * x[i] - 10.0 * cos(2.0 * M_PI * x[i]);
//     }
//     return 10.0 * NUM_VARIABLES + sum;
// }

// Beale function (2 variables only). Set Lbound = -4.5 and Ubound = 4.5
// double Objective_function(int NUM_VARIABLES, double x[NUM_VARIABLES])
// {
//     if (NUM_VARIABLES != 2) {
//         // Beale function is defined for two variables only
//         return -1.0; // Error value
//     }

//     double x1 = x[0];
//     double x2 = x[1];

//     double term1 = 1.5 - x1 + x1 * x2;
//     double term2 = 2.25 - x1 + x1 * x2 * x2;
//     double term3 = 2.625 - x1 + x1 * x2 * x2 * x2;

//     return (term1 * term1) + (term2 * term2) + (term3 * term3);
// }

// Levi function N. 13 (2 variables only). Set Lbound = -10.0 and Ubound = 10.0
// double Objective_function(int NUM_VARIABLES, double x[NUM_VARIABLES]) {
//     if (NUM_VARIABLES != 2) {
//         // Handle the case where the number of variables is not 2
//         return 0.0;  // You may want to use a different value or handle it differently based on your requirements.
//     }

//     double x1 = x[0];
//     double x2 = x[1];

//     // Levi function N.13
//     double term1 = pow(sin(3 * M_PI * x1), 2);
//     double term2 = pow(x1 - 1, 2) * (1 + pow(sin(3 * M_PI * x2), 2));
//     double term3 = pow(x2 - 1, 2) * (1 + pow(sin(2 * M_PI * x2), 2));

//     return term1 + term2 + term3;
// }

// Himmelblau function (2 variables only). Set Lbound = -5.0 and Ubound = 5.0
// double Objective_function(int NUM_VARIABLES, double x[NUM_VARIABLES]) {
//     // Himmelblau's function
//     double term1 = (x[0]*x[0] + x[1] - 11)*(x[0]*x[0] + x[1] - 11);
//     double term2 = (x[0] + x[1]*x[1] - 7)*(x[0] + x[1]*x[1] - 7);
//     return term1 + term2;
// }

// Easom function (2 variables only). Set Lbound = -100.0 and Ubound = 100.0
// double Objective_function(int NUM_VARIABLES, double x[NUM_VARIABLES]) {
//     // Ensure that NUM_VARIABLES is 2
//     if (NUM_VARIABLES != 2) {
//         // Handle the error accordingly (e.g., print an error message or return an error code)
//         return 0.0; // You may choose a proper error value
//     }

//     // Extract the variables x1 and x2
//     double x1 = x[0];
//     double x2 = x[1];

//     // Calculate the Easom function
//     double result = -cos(x1) * cos(x2) * exp(-(pow(x1 - M_PI, 2) + pow(x2 - M_PI, 2)));

//     return result;
// }