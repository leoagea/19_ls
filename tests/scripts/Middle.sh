#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

echo -e "\n${RED}Recursive Listing and Multiple Options Test${RESET}\n"

# Initialize counters
total_tests=0
successful_tests=0

# Test environment setup
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
TEST_DIR="/tmp/ls_test_recursive"
mkdir -p "$TEST_DIR"
cp ../myls "$TEST_DIR/myls"
cd "$TEST_DIR"

# Test cases arrays
SINGLE_TESTS=(
    "R"                     # Recursive listing
    "l"                     # Long format
    "a"                     # Show hidden files
)

COMBINED_TESTS=(
    "lt"                    # Long format, time sorted
    "la"                    # Long format, show hidden
    "lR"                    # Long format, recursive
    "laR"                   # Long format, hidden, recursive
)

SEPARATE_TESTS=(
    "l -t"                  # Long format, time sorted
    "l -a"                  # Long format, show hidden
    "l -R"                  # Long format, recursive
    "l -a -R"              # Long format, hidden, recursive
)

PATH_TESTS=(
    "R dir1 dir2"          # Recursive on specific dirs
    "lt dir1"              # Long format, time sorted on dir1
    "lR dir1 dir2"         # Long format, recursive on dirs
    "laR dir1"             # Long format, hidden, recursive on dir1
)

# Function to create test environment
setup_test_env() {
    echo -e "\n${BLUE}Creating test environment...${RESET}"
    
    # Create complex directory structure
    mkdir -p dir1/subdir1/subsubdir1
    mkdir -p dir1/subdir2
    mkdir -p dir2/subdir1
    mkdir -p dir3

    # Create files with different timestamps and permissions
    echo "content1" > dir1/file1.txt
    chmod 644 dir1/file1.txt
    sleep 1

    echo "content2" > dir1/subdir1/file2.txt
    chmod 755 dir1/subdir1/file2.txt
    sleep 1

    echo "content3" > dir1/subdir2/file3.txt
    chmod 600 dir1/subdir2/file3.txt
    sleep 1

    echo "hidden1" > dir1/.hidden1
    chmod 644 dir1/.hidden1
    sleep 1

    echo "content4" > dir2/file4.txt
    chmod 644 dir2/file4.txt
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

# Function to test single flag
test_single_flag() {
    local flag=$1
    echo -e "\n${YELLOW}Testing -$flag${RESET}"
    diff_output=$(diff <(ls -$flag 2>/dev/null) <(./myls -$flag 2>/dev/null))
    check_diff "$diff_output" "ls -$flag"
}

# Function to test combined flags
test_combined_flags() {
    local flags=$1
    echo -e "\n${YELLOW}Testing -$flags${RESET}"
    diff_output=$(diff <(ls -$flags 2>/dev/null) <(./myls -$flags 2>/dev/null))
    check_diff "$diff_output" "ls -$flags"
}

# Function to test separate flags
test_separate_flags() {
    local flags=$1
    echo -e "\n${YELLOW}Testing ls -$flags${RESET}"
    diff_output=$(diff <(ls -$flags 2>/dev/null) <(./myls -$flags 2>/dev/null))
    check_diff "$diff_output" "ls -$flags"
}

# Function to test with paths
test_with_paths() {
    local args=$1
    echo -e "\n${YELLOW}Testing ls -$args${RESET}"
    diff_output=$(diff <(ls -$args 2>/dev/null) <(./myls -$args 2>/dev/null))
    check_diff "$diff_output" "ls -$args"
}

# Main testing sequence
main() {
    echo -e "${BLUE}Starting comprehensive tests${RESET}"
    setup_test_env
    
    # Test single flags
    echo -e "\n${BLUE}Testing single flags${RESET}"
    for test in "${SINGLE_TESTS[@]}"; do
        test_single_flag "$test"
    done
    
    # Test combined flags
    echo -e "\n${BLUE}Testing combined flags${RESET}"
    for test in "${COMBINED_TESTS[@]}"; do
        test_combined_flags "$test"
    done
    
    # Test separate flags
    echo -e "\n${BLUE}Testing separate flags${RESET}"
    for test in "${SEPARATE_TESTS[@]}"; do
        test_separate_flags "$test"
    done
    
    # Test with paths
    echo -e "\n${BLUE}Testing with paths${RESET}"
    for test in "${PATH_TESTS[@]}"; do
        test_with_paths "$test"
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