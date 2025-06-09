#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

echo -e "\n${RED}ACL and Extended Attributes Test${RESET}\n"

# Initialize counters
total_tests=0
successful_tests=0

# Test environment setup
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
TEST_DIR="/tmp/ls_test_acl"
mkdir -p "$TEST_DIR"
cp myls "$TEST_DIR/myls"
cd "$TEST_DIR"

# Test cases arrays
ACL_TESTS=(
    "l acl_file"                    # Basic ACL file listing
    "l acl_dir"                     # Basic ACL directory listing
    "la acl_file"                   # Detailed ACL file listing
)

XATTR_TESTS=(
    "l@ xattr_file"                 # Basic xattr file listing
    "l@ both_file"                  # File with both ACL and xattr
)

MIXED_TESTS=(
    "l@ regular_file acl_file xattr_file both_file"    # Multiple files
    "l regular_dir"                                     # Directory listing
)

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

# Function to create test environment
setup_test_env() {
    echo -e "\n${BLUE}Creating test environment...${RESET}"
    
    # Check for required commands
    if ! command -v setfacl &> /dev/null || ! command -v attr &> /dev/null; then
        echo "Error: This test requires 'acl' and 'attr' packages. Please install them first:"
        echo "sudo apt-get install acl attr"
        exit 1
    fi
    
    # Create files and directories
    echo "content1" > regular_file
    echo "content2" > acl_file
    echo "content3" > xattr_file
    echo "content4" > both_file
    mkdir regular_dir
    mkdir acl_dir

    # Set up ACLs
    echo -e "\n${BLUE}Setting up ACLs...${RESET}"
    setfacl -m u:nobody:r-- acl_file
    setfacl -m u:nobody:r-x acl_dir
    setfacl -m g:daemon:r-- both_file

    # Set up extended attributes
    echo -e "\n${BLUE}Setting up extended attributes...${RESET}"
    attr -s "user.test" -V "test_value" xattr_file
    attr -s "user.owner" -V "tester" both_file
    attr -s "user.project" -V "ls_test" both_file
}

# Function to test ACL features
test_acl() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls -$test_case${RESET}"
    diff_output=$(diff <(ls -$test_case 2>/dev/null) <(DOCKER_BUILD=1 ./myls -$test_case 2>/dev/null))
    check_diff "$diff_output" "ls -$test_case"
}

# Function to test extended attributes
test_xattr() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls -$test_case${RESET}"
    diff_output=$(diff <(ls -$test_case 2>/dev/null) <(./myls -$test_case 2>/dev/null))
    check_diff "$diff_output" "ls -$test_case"
}

# Function to test mixed scenarios
test_mixed() {
    local test_case=$1
    echo -e "\n${YELLOW}Testing ls -$test_case${RESET}"
    diff_output=$(diff <(ls -$test_case 2>/dev/null) <(./myls -$test_case 2>/dev/null))
    check_diff "$diff_output" "ls -$test_case"
}

# Main testing sequence
main() {
    echo -e "${BLUE}Starting ACL and Extended Attributes tests${RESET}"
    setup_test_env
    
    # Test ACL features
    echo -e "\n${BLUE}Testing ACL features${RESET}"
    for test in "${ACL_TESTS[@]}"; do
        test_acl "$test"
    done
    
    # Test extended attributes
    echo -e "\n${BLUE}Testing extended attributes${RESET}"
    for test in "${XATTR_TESTS[@]}"; do
        test_xattr "$test"
    done
    
    # Test mixed scenarios
    echo -e "\n${BLUE}Testing mixed scenarios${RESET}"
    for test in "${MIXED_TESTS[@]}"; do
        test_mixed "$test"
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
    # Remove ACLs and extended attributes before deletion
    setfacl -b regular_dir/* 2>/dev/null
    setfacl -b regular_dir acl_dir 2>/dev/null
    cd - > /dev/null 2>&1
    rm -rf "$TEST_DIR"
}

# Set cleanup trap
trap cleanup EXIT

# Run tests
main