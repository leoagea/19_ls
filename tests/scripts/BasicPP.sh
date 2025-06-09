#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

echo -e "\n${RED}Recursive and Time Sorting Test${RESET}\n"

# Initialize counters
total_tests=0
successful_tests=0

# Test environment setup
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
TEST_DIR="/tmp/ls_test_rt"
mkdir -p "$TEST_DIR"
cp ../myls "$TEST_DIR/myls"
cd "$TEST_DIR"

# Test cases arrays
BASIC_TESTS=(
    "r"                                 # Reverse sort
    "t"                                # Time sort
)

MULTI_PARAM_TESTS=(
    "r dir1 dir2 dir3"                 # Reverse with multiple dirs
    "t dir1 dir2 dir3"                 # Time sort with multiple dirs
    "r dir1/subdir1 dir2/subdir1"      # Reverse with nested dirs
    "t dir1/subdir1 dir2/subdir1"      # Time sort with nested dirs
)

# Function to create test environment
setup_test_env() {
    echo -e "\n${BLUE}Creating test environment...${RESET}"
    
    # Create directories with nested structure
    mkdir -p dir1/subdir1
    mkdir -p dir1/subdir2
    mkdir -p dir2/subdir1
    mkdir -p dir3

    # Create files with different timestamps
    echo "content1" > dir1/file1.txt
    sleep 1
    echo "content2" > dir1/subdir1/file2.txt
    sleep 1
    echo "content3" > dir1/subdir2/file3.txt
    sleep 1
    echo "content4" > dir2/file4.txt
    sleep 1
    echo "content5" > dir2/subdir1/file5.txt
    sleep 1
    echo "content6" > dir3/file6.txt
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
        echo -e "${RED}✗ $test_name:${RESET}"
        echo "$diff_output"
    fi
}

# Function to test single flag
test_single_flag() {
    local flag=$1
    echo -e "\n${YELLOW}Testing -$flag${RESET}"
    # echo "Real ls -$flag output:"
    # ls -$flag
    # echo -e "\nMy ls -$flag output:"
    # ./myls -$flag
    
    diff_output=$(diff <(ls -$flag 2>/dev/null) <(./myls -$flag 2>/dev/null))
    check_diff "$diff_output" "ls -$flag"
}

# Function to test with parameters
test_with_params() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls -$test_case${RESET}"
    # echo "Real ls -$test_case output:"
    # ls -$test_case
    # echo -e "\nMy ls -$test_case output:"
    # ./myls -$test_case
    
    diff_output=$(diff <(ls -$test_case 2>/dev/null) <(./myls -$test_case 2>/dev/null))
    check_diff "$diff_output" "ls -$test_case"
}

# Main testing sequence
main() {
    echo -e "${BLUE}Starting recursive and time sorting tests${RESET}"
    setup_test_env
    
    # Test single flags
    echo -e "\n${BLUE}Testing basic flags${RESET}"
    for test in "${BASIC_TESTS[@]}"; do
        test_single_flag "$test"
    done
    
    # Test with multiple parameters
    echo -e "\n${BLUE}Testing with parameters${RESET}"
    for test in "${MULTI_PARAM_TESTS[@]}"; do
        test_with_params "$test"
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