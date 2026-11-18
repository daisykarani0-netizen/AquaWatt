/*
 * Project: AquaWatt - Personal Utility Calculator
 * Course: Software Development and Operations
 * Description: Calculates water and electricity costs,
 *              checks a monthly budget, and estimates savings.
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <math.h>

/*
 * Function: parse_number
 * Converts a command-line argument into a double.
 * Returns 1 for a valid non-negative number and 0 otherwise.
 */
int parse_number(const char *text, double *value) {
    char *end;

    errno = 0;
    *value = strtod(text, &end);

    if (end == text || *end != '\0' ||
        errno != 0 || !isfinite(*value) || *value < 0) {
        return 0;
    }

    return 1;
}

/*
 * Function: calculate_water
 * Uses tiered water pricing:
 * First 10 cubic meters cost 2.50 EUR per m3.
 * Additional cubic meters cost 3.50 EUR per m3.
 */
double calculate_water(double usage) {
    if (usage <= 10) {
        return usage * 2.50;
    } else {
        return (10 * 2.50) + ((usage - 10) * 3.50);
    }
}

/*
 * Function: calculate_electricity
 * Calculates electricity cost using a fixed rate
 * of 0.30 EUR per kilowatt-hour.
 */
double calculate_electricity(double kwh) {
    return kwh * 0.30;
}

/*
 * Function: calculate_total
 * Adds water and electricity expenses together.
 */
double calculate_total(double water_cost,
                       double electricity_cost) {
    return water_cost + electricity_cost;
}

/*
 * Function: calculate_savings
 * Calculates how much money would be saved
 * if both utilities were reduced by 10%.
 */
double calculate_savings(double water, double electricity,
                         double current_total) {
    double new_water = calculate_water(water * 0.90);
    double new_electricity =
        calculate_electricity(electricity * 0.90);

    return current_total - (new_water + new_electricity);
}

/*
 * Function: check_budget
 * Compares the total utility cost with the user's budget.
 */
void check_budget(double total, double budget) {
    if (total > budget) {
        printf("Over budget by: %.2f EUR\n", total - budget);
    } else {
        printf("Remaining budget: %.2f EUR\n", budget - total);
    }
}

/*
 * Main function:
 * Reads three command-line arguments:
 * 1. Water consumption in m3
 * 2. Electricity consumption in kWh
 * 3. Monthly budget in EUR
 */
int main(int argc, char *argv[]) {
    double water, electricity, budget;

    // Check that the correct number of arguments was entered.
    if (argc != 4) {
        fprintf(stderr,
            "Usage: %s water_m3 electricity_kwh budget\n",
            argv[0]);
        return 1;
    }

    // Convert inputs and reject invalid or negative values.
    if (!parse_number(argv[1], &water) ||
        !parse_number(argv[2], &electricity) ||
        !parse_number(argv[3], &budget)) {
        fprintf(stderr,
            "Error: Enter valid non-negative numbers.\n");
        return 1;
    }

    // Calculate the individual utility costs.
    double water_cost = calculate_water(water);
    double electricity_cost =
        calculate_electricity(electricity);

    // Calculate the combined monthly expense.
    double total = calculate_total(water_cost,
                                   electricity_cost);

    // Estimate savings from reducing consumption by 10%.
    double savings =
        calculate_savings(water, electricity, total);

    // Check for numbers too large to calculate safely.
    if (!isfinite(total) || !isfinite(savings)) {
        fprintf(stderr, "Error: Calculation overflow.\n");
        return 1;
    }

    // Display the calculated results.
    printf("===== AquaWatt Utility Calculator =====\n");
    printf("Water cost: %.2f EUR\n", water_cost);
    printf("Electricity cost: %.2f EUR\n", electricity_cost);
    printf("Total cost: %.2f EUR\n", total);

    // Compare expenses with the monthly budget.
    check_budget(total, budget);

    // Display potential savings.
    printf("Savings with 10%% less usage: %.2f EUR\n",
           savings);

    return 0;
}