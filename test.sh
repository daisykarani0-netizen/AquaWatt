#!/bin/bash

# Project: AquaWatt
# Course: Software Development and Operations
# Description: Automated tests for the AquaWatt C program.

# Stop if compilation fails.
set -e

# Compile the C program.
gcc -Wall -Wextra aquawatt.c -o aquawatt -lm

# Keep track of passed and failed tests.
passed=0
failed=0

# Function to test a successful calculation.
run_test() {
    description="$1"
    expected="$2"
    shift 2

    # Run AquaWatt using the provided arguments.
    output=$(./aquawatt "$@")

    # Check whether the expected result appears.
    if echo "$output" | grep -Fq "$expected"; then
        echo "PASS: $description"
        passed=$((passed + 1))
    else
        echo "FAIL: $description"
        echo "Expected: $expected"
        echo "Got: $output"
        failed=$((failed + 1))
    fi
}

# Function to check that invalid inputs are rejected.
run_error_test() {
    description="$1"
    shift

    # A nonzero exit status means the program rejected the input.
    if ./aquawatt "$@" >/dev/null 2>&1; then
        echo "FAIL: $description"
        failed=$((failed + 1))
    else
        echo "PASS: $description"
        passed=$((passed + 1))
    fi
}

# Test 1: Normal water and electricity calculation.
run_test "Normal calculation" "Total cost: 78.50 EUR" 15 120 80

# Test 2: Both consumption values are zero.
run_test "Zero consumption" "Total cost: 0.00 EUR" 0 0 50

# Test 3: Exactly 10 cubic meters of water.
run_test "First water tier" "Water cost: 25.00 EUR" 10 0 50

# Test 4: Water usage exceeds the first tier.
run_test "Second water tier" "Water cost: 60.00 EUR" 20 0 100

# Test 5: The user exceeds their monthly budget.
run_test "Over budget" "Over budget by: 28.50 EUR" 15 120 50

# Test 6: Check savings when consumption decreases by 10%.
run_test "Savings calculation" "Savings with 10% less usage: 8.85 EUR" 15 120 80

# Test 7: Reject negative water consumption.
run_error_test "Negative input" -5 100 80

# Test 8: Reject missing arguments.
run_error_test "Missing argument" 15 120

# Test 9: Reject text instead of a number.
run_error_test "Invalid input" abc 100 80

# Display the final test summary.
echo "-----------------------------"
echo "Passed: $passed"
echo "Failed: $failed"

# Return failure if any test did not pass.
if [ "$failed" -gt 0 ]; then
    exit 1
fi

echo "All tests passed!"
exit 0