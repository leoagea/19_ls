#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

echo -e "\n${RED}Error Handling Test${RESET}\n"

# Initialize counters
total_tests=0
successful_tests=0

# Test environment setup
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
TEST_DIR="/tmp/ls_test_errors"
mkdir -p "$TEST_DIR"
cp ../myls "$TEST_DIR/myls"
cd "$TEST_DIR"

# Test cases arrays
NONEXISTENT_TESTS=(
    "nonexistent_file"                          # Single nonexistent file
    "nonexistent_dir"                           # Single nonexistent directory
    "accessible_file nonexistent_file"          # Mix of existent and nonexistent
)

INACCESSIBLE_TESTS=(
    "no_access_file"                            # Inaccessible file
    "no_access_dir"                             # Inaccessible directory
    "-l no_access_file"                         # Long format with inaccessible file
)

INVALID_OPTIONS_TESTS=(
    "z"                                         # Single invalid option
    "2yz"                                       # Multiple invalid options
    "la -z"                                     # Mix of valid and invalid options
)

COMPLEX_TESTS=(
    "z nonexistent_file"                        # Invalid option with nonexistent file
    "l no_access_file"                          # Valid option with inaccessible file
)

# Function to create test environment
setup_test_env() {
    echo -e "\n${BLUE}Creating test environment...${RESET}"
    
    # Create regular file and directory
    mkdir accessible_dir
    echo "content" > accessible_file

    # Create inaccessible file and directory
    mkdir no_access_dir
    echo "secret" > no_access_file
    chmod 000 no_access_dir
    chmod 000 no_access_file
}

# Function to check diff and update counters
check_diff() {
    local diff_output=$1
    local test_name=$2
    ((total_tests++))
    
    if [ -z "$diff_output" ]; then
        echo -e "${GREEN}✓ $test_name${RESET}"
        ((successful_tests++))
    else
        echo -e "${RED}✗ $test_name${RESET}"
        echo "$diff_output"
    fi
}

# Function to test nonexistent files/folders
test_nonexistent() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls $test_case${RESET}"
    diff_output=$(diff <(ls $test_case 2>&1) <(./myls $test_case 2>&1))
    check_diff "$diff_output" "ls $test_case"
}

# Function to test inaccessible files/folders
test_inaccessible() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls $test_case${RESET}"
    diff_output=$(diff <(ls $test_case 2>&1) <(./myls $test_case 2>&1))
    check_diff "$diff_output" "ls $test_case"
}

# Function to test invalid options
test_invalid_options() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls -$test_case${RESET}"
    diff_output=$(diff <(ls -$test_case 2>&1) <(./myls -$test_case 2>&1))
    check_diff "$diff_output" "ls -$test_case"
}

# Function to test complex cases
test_complex() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls -$test_case${RESET}"
    diff_output=$(diff <(ls -$test_case 2>&1) <(./myls -$test_case 2>&1))
    check_diff "$diff_output" "ls -$test_case"
}

# Main testing sequence
main() {
    echo -e "${BLUE}Starting error handling tests${RESET}"
    setup_test_env
    
    # Test nonexistent files/folders
    echo -e "\n${BLUE}Testing nonexistent files/folders${RESET}"
    for test in "${NONEXISTENT_TESTS[@]}"; do
        test_nonexistent "$test"
    done
    
    # Test inaccessible files/folders
    echo -e "\n${BLUE}Testing inaccessible files/folders${RESET}"
    for test in "${INACCESSIBLE_TESTS[@]}"; do
        test_inaccessible "$test"
    done
    
    # Test invalid options
    echo -e "\n${BLUE}Testing invalid options${RESET}"
    for test in "${INVALID_OPTIONS_TESTS[@]}"; do
        test_invalid_options "$test"
    done
    
    # Test complex cases
    echo -e "\n${BLUE}Testing complex error cases${RESET}"
    for test in "${COMPLEX_TESTS[@]}"; do
        test_complex "$test"
    done
    
    # Print summary
    echo -e "\n${BLUE}Test Summary:${RESET}"
    echo "Total tests: $total_tests"
    echo "Successful tests: $successful_tests"
    echo "Failed tests: $((total_tests - successful_tests))"
    
    if [ $total_tests -eq $successful_tests ]; then
        echo -e "${GREEN}All tests passed!${RESET}"
    else
        echo -e "${RED}Some tests failed!${RESET}"
    fi
}

# Cleanup function
cleanup() {
    chmod 755 no_access_dir
    chmod 644 no_access_file
    cd - > /dev/null 2>&1
    rm -rf "$TEST_DIR"
}

# Set cleanup trap
trap cleanup EXIT

# Run tests
main