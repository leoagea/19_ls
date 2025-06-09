#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

echo -e "\n${RED}File Types Test${RESET}\n"

# Initialize counters
total_tests=0
successful_tests=0

# Test environment setup
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
TEST_DIR="/tmp/ls_test_types"
mkdir -p "$TEST_DIR"
cp myls "$TEST_DIR/myls"
cd "$TEST_DIR"

# Test cases arrays
BASIC_TESTS=(
    "l"                     # Basic long format
    "la"                    # Show hidden files
)

FILETYPE_TESTS=(
    "l testchar"           # Character device
    "l testblock"          # Block device
    "l testpipe"           # Named pipe
    "l testsock"           # Unix socket
    "l testlink"           # Symbolic link
)

COMBINED_TESTS=(
    "la testchar testblock testpipe"    # Multiple special files
    "lR"                                # Recursive with special files
)

# Function to create test environment
setup_test_env() {
    echo -e "\n${BLUE}Creating test environment...${RESET}"
    
    # Create regular files
    echo "content" > regular_file
    
    # Create a named pipe (FIFO)
    mkfifo testpipe
    
    # Create special files (needs root)
    if [ "$EUID" -eq 0 ]; then
        mknod testchar c 1 3    # Major number 1, minor number 3 for null device
        mknod testblock b 8 0   # Major number 8, minor number 0 for first SCSI disk
    else
        echo "${YELLOW}Warning: Not running as root, skipping device files creation${RESET}"
    fi
    
    # Create a Unix domain socket
    python3 -c "import socket; s = socket.socket(socket.AF_UNIX); s.bind('testsock')"
    
    # Create a symbolic link
    ln -s regular_file testlink
    
    # Create directories
    mkdir testdir
    mkdir -p nested/dir/structure
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

# Function to test basic features
test_basic() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls -$test_case${RESET}"
    diff_output=$(diff <(ls -$test_case 2>/dev/null) <(./myls -$test_case 2>/dev/null))
    check_diff "$diff_output" "ls -$test_case"
}

# Function to test specific file types
test_filetype() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls -$test_case${RESET}"
    diff_output=$(diff <(ls -$test_case 2>/dev/null) <(./myls -$test_case 2>/dev/null))
    check_diff "$diff_output" "ls -$test_case"
}

# Function to test combined scenarios
test_combined() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls -$test_case${RESET}"
    diff_output=$(diff <(ls -$test_case 2>/dev/null) <(./myls -$test_case 2>/dev/null))
    check_diff "$diff_output" "ls -$test_case"
}

# Main testing sequence
main() {
    echo -e "${BLUE}Starting perm file tests${RESET}"
    setup_test_env
    
    # Test basic features
    echo -e "\n${BLUE}Testing basic features${RESET}"
    for test in "${BASIC_TESTS[@]}"; do
        test_basic "$test"
    done
    
    # Test file types
    echo -e "\n${BLUE}Testing specific file types${RESET}"
    for test in "${FILETYPE_TESTS[@]}"; do
        test_filetype "$test"
    done
    
    # Test combined scenarios
    echo -e "\n${BLUE}Testing combined scenarios${RESET}"
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