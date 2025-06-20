#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

echo -e "\n${RED}Size Format Test (-h flag)${RESET}\n"

# Initialize counters
total_tests=0
successful_tests=0

# Test environment setup
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
TEST_DIR="/tmp/ls_test_size"
mkdir -p "$TEST_DIR"
cp ../myls "$TEST_DIR/myls"
cd "$TEST_DIR"

# Test cases arrays
SIZE_TESTS=(
    "lh"                    # Basic human readable
    "lah"                   # All files with human readable
    "lhs"                   # Human readable with block size
)

FILE_SPECIFIC_TESTS=(
    "lh empty_file"        # Test with empty file
    "lh huge_file"         # Test with huge file
    "lh symlink_file"      # Test with symlink
)

COMBINED_TESTS=(
    "lh empty_file small_file medium_file"     # Multiple files
    "lhr"                                      # Reverse with human readable
    "lht"                                      # Time sort with human readable
)

# Function to create test environment
setup_test_env() {
    echo -e "\n${BLUE}Creating test environment...${RESET}"
    
    # Empty file (0B)
    touch empty_file

    # Small file (1K)
    dd if=/dev/zero of=small_file bs=1K count=1 2>/dev/null

    # Medium file (10M)
    dd if=/dev/zero of=medium_file bs=1M count=10 2>/dev/null

    # Large file (100M)
    dd if=/dev/zero of=large_file bs=1M count=100 2>/dev/null

    # Huge file (1.5G)
    dd if=/dev/zero of=huge_file bs=1M count=1500 2>/dev/null

    # Create symbolic link
    ln -s large_file symlink_file
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

# Function to test size formatting
test_size() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls -$test_case${RESET}"
    diff_output=$(diff <(ls -$test_case 2>/dev/null) <(./myls -$test_case 2>/dev/null))
    check_diff "$diff_output" "ls -$test_case"
}

# Function to test specific files
test_specific_file() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls -$test_case${RESET}"
    diff_output=$(diff <(ls -$test_case 2>/dev/null) <(./myls -$test_case 2>/dev/null))
    check_diff "$diff_output" "ls -$test_case"
}

# Function to test combined flags
test_combined() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls -$test_case${RESET}"
    diff_output=$(diff <(ls -$test_case 2>/dev/null) <(./myls -$test_case 2>/dev/null))
    check_diff "$diff_output" "ls -$test_case"
}

# Main testing sequence
main() {
    echo -e "${BLUE}Starting size format tests${RESET}"
    setup_test_env
    
    echo -e "\nTest with real ls command:"
    ls -lhs
    echo -e "\nTest with myls command:"
    ./myls -lhs

    
    # Test size formatting
    echo -e "\n${BLUE}Testing basic size formatting${RESET}"
    for test in "${SIZE_TESTS[@]}"; do
        test_size "$test"
    done
    
    # Test specific files
    echo -e "\n${BLUE}Testing specific files${RESET}"
    for test in "${FILE_SPECIFIC_TESTS[@]}"; do
        test_specific_file "$test"
    done
    
    # Test combined flags
    echo -e "\n${BLUE}Testing combined flags${RESET}"
    for test in "${COMBINED_TESTS[@]}"; do
        test_combined "$test"
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
    cd - > /dev/null 2>&1
    rm -rf "$TEST_DIR"
}

# Set cleanup trap
trap cleanup EXIT

# Run tests
main