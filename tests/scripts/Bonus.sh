#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

# Initialize counters
total_tests=0
successful_tests=0

# Test environment setup
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
TEST_DIR="/tmp/ls_test_flags"
mkdir -p "$TEST_DIR"
cp ../myls "$TEST_DIR/myls"
cd "$TEST_DIR"

# All flags to test
FLAGS=(
    "a" "r" "t" "R" "l" "S"
	"s" "f" "g" "n" "p" "1"
	"u" "d" "h"
)

# Function to create test environment
setup_test_env() {
    echo -e "\n${BLUE}Creating test environment...${RESET}"
    
    # Create various types of files and directories
    mkdir -p dir1/subdir1 dir2/.hidden
    echo "content1" > file1.txt
    echo "content2" > .hidden_file
    echo "content3" > dir1/file2.txt
    ln -s file1.txt symlink1
    
    # Create files with different timestamps
    touch -t 202001010000 old_file
    touch -t 202312312359 new_file
    
    # Create files with different sizes
    dd if=/dev/zero of=large_file bs=1M count=1 2>/dev/null
    dd if=/dev/zero of=small_file bs=1K count=1 2>/dev/null
    
    # Set up ACL if available
    if command -v setfacl >/dev/null; then
        setfacl -m u:nobody:r-- file1.txt
    fi
    
    # Set up extended attributes if available
    if command -v attr >/dev/null; then
        attr -s "user.test" -V "test_value" file1.txt 2>/dev/null
    fi
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
    diff_output=$(diff <(ls -$ls_flag 2>/dev/null) <(./myls -$flag 2>/dev/null))
    check_diff "$diff_output" "ls -$flag"
}

# Function to test flag combinations
test_flag_combination() {
    local flags=$1
    echo -e "\n${YELLOW}Testing -$flags${RESET}"
    diff_output=$(diff <(ls -$flags 2>/dev/null) <(./myls -$flags 2>/dev/null))
    check_diff "$diff_output" "ls -$flags"
}

# Main testing sequence
main() {
    echo -e "${BLUE}Starting comprehensive flag tests${RESET}"
    setup_test_env

    # Test individual flags
    for flag in "${FLAGS[@]}"; do
        test_single_flag "$flag"
    done
    
    # Test common flag combinations
    echo -e "\n${BLUE}Testing common flag combinations${RESET}"
    COMBINATIONS=(
        "la"    # Long format with hidden files
        "lR"    # Recursive long format
        "lt"    # Time sorted long format
        "lrt"   # Reverse time sorted long format
        "lS"    # Size sorted long format
        "lh"    # Human readable sizes
        "lan"   # Numeric UIDs/GIDs
        "laR"   # Recursive with hidden files
        "ltr"   # Reverse time sort
        "ls"    # Long format with block size
    )
    
    for combo in "${COMBINATIONS[@]}"; do
        test_flag_combination "$combo"
    done
    
    # Test complex combinations
    echo -e "\n${BLUE}Testing complex combinations${RESET}"
    COMPLEX_COMBINATIONS=(
        "laRt"      # Recursive, all files, time sorted
        "laSrh"     # Size sorted, human readable, reverse
        "lan1"      # Numeric IDs, one per line
        "latRS"     # All files, time sorted, recursive, size sorted
        "lhaSr"     # Human readable, all files, size sorted, reverse
    )
    
    for combo in "${COMPLEX_COMBINATIONS[@]}"; do
        test_flag_combination "$combo"
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