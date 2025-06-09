#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

echo -e "\n${RED}Basic Features Test${RESET}\n"

# Initialize counters
total_tests=0
successful_tests=0

# Test environment setup
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
TEST_DIR="/tmp/ls_test_basic"
mkdir -p "$TEST_DIR"
cp ../myls "$TEST_DIR/myls"
cd "$TEST_DIR"

# Function to create test environment
setup_test_env() {
    echo -e "\n${BLUE}Creating test environment...${RESET}"
    
    # Create regular files
    touch file1.txt
    echo "Hello" > file2.txt
    echo "World" > file3.txt
    
    # Create hidden files
    touch .hidden1
    touch .hidden2
    
    # Create directories
    mkdir test_dir
    mkdir .hidden_dir
    mkdir -p nested/dir/structure
    
    # Create symbolic links
    ln -s file1.txt symlink1
    ln -s test_dir dir_symlink
    ln -s nonexistent_file broken_link
    
    # Create files with different timestamps
    touch -t 202001010000 old_file
    touch -t 202312312359 new_file
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

# Basic test cases
BASIC_TESTS=(
    ""        # No flags
    "l"       # Long format
    "a"       # Show hidden
    "la"      # Long format with hidden
    "lR"      # Recursive
    "laR"     # Recursive with hidden
)

# Symlink specific tests
SYMLINK_TESTS=(
    "l symlink1"
    "l dir_symlink"
    "l broken_link"
    "lR symlink1"
)

# Main testing sequence
main() {
    echo -e "${BLUE}Starting basic feature tests${RESET}"
    setup_test_env
    
    # Test basic functionality
    echo -e "\n${BLUE}Testing basic functionality${RESET}"
    for test in "${BASIC_TESTS[@]}"; do
        if [ -z "$test" ]; then
            echo -e "\n${YELLOW}Testing ls with no flags${RESET}"
            diff_output=$(diff <(ls) <(./myls))
            check_diff "$diff_output" "ls (no flags)"
        else
            echo -e "\n${YELLOW}Testing ls -$test${RESET}"
            diff_output=$(diff <(ls -$test 2>/dev/null) <(./myls -$test 2>/dev/null))
            check_diff "$diff_output" "ls -$test"
        fi
    done
    
    # Test symlink handling
    echo -e "\n${BLUE}Testing symlink handling${RESET}"
    for test in "${SYMLINK_TESTS[@]}"; do
        echo -e "\n${YELLOW}Testing ls -$test${RESET}"
        diff_output=$(diff <(ls -$test 2>/dev/null) <(./myls -$test 2>/dev/null))
        check_diff "$diff_output" "ls -$test"
    done
    
    # Test error handling
    echo -e "\n${BLUE}Testing error handling${RESET}"
    diff_output=$(diff <(ls nonexistent 2>&1) <(./myls nonexistent 2>&1))
    check_diff "$diff_output" "Nonexistent file error"
    
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