#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
RESET=$(tput sgr0)

echo -e "\n${RED}ACL and Extended Attributes Test${RESET}\n"

# Initialize counters
total_tests=0
successful_tests=0

# Get the directory where the script is located
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
PROJECT_DIR=$(dirname "$SCRIPT_DIR")

# Create temporary directory
mkdir -p /tmp/ls_test_acl
cp myls /tmp/ls_test_acl/myls > /dev/null 2>&1
chmod +x /tmp/ls_test_acl/myls > /dev/null 2>&1
cd /tmp/ls_test_acl

pwd
ls -l

# Function to check diff and update counters
check_diff() {
    local diff_output=$1
    local test_name=$2
    ((total_tests++))
    
    if [ -z "$diff_output" ]; then
        echo -e "${GREEN}No differences found for $test_name. My ls works correctly!${RESET}"
        ((successful_tests++))
    else
        echo -e "${RED}Differences found for $test_name:${RESET}"
        echo "$diff_output"
    fi
}

# Check if setfacl and attr commands are available
if ! command -v setfacl &> /dev/null || ! command -v attr &> /dev/null; then
    echo "Error: This test requires 'acl' and 'attr' packages. Please install them first:"
    echo "sudo apt-get install acl attr"
    exit 1
fi

# Create test environment
echo -e "\n${BLUE}Creating test environment...${RESET}"

# Create files and directories for testing
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

# Test regular ls -l (should show + for ACL)
echo -e "\n${BLUE}Testing basic ACL indicator${RESET}"
echo "Testing ls -l with ACL file:"
# ls -l acl_file
# echo -e "\nMy ls -l with ACL file:"
# ./myls -l acl_file
diff_output=$(diff <(ls -l acl_file) <(DOCKER_BUILD=1 ./myls -l acl_file))
check_diff "$diff_output" "ACL file basic listing"

echo "Testing ls -l with ACL directory:"
diff_output=$(diff <(ls -l acl_dir) <(DOCKER_BUILD=1 ./myls -l acl_dir))
check_diff "$diff_output" "ACL directory basic listing"

# Test ls with ACL details
echo -e "\n${BLUE}Testing detailed ACL listings${RESET}"
if ls --version 2>&1 | grep -q 'GNU'; then
    # GNU ls specific tests
    echo "Testing ls -la with ACL file:"
    diff_output=$(diff <(ls -la acl_file) <(./myls -la acl_file))
    check_diff "$diff_output" "ACL file detailed listing"
fi

# Test extended attributes indicator
echo -e "\n${BLUE}Testing extended attributes${RESET}"
echo "Testing ls -l with xattr file:"
diff_output=$(diff <(ls -l xattr_file) <(./myls -l@ xattr_file))
check_diff "$diff_output" "xattr file listing"

# Test file with both ACL and xattr
echo -e "\n${BLUE}Testing combined ACL and xattr${RESET}"
echo "Testing ls -l with combined attributes:"
diff_output=$(diff <(ls -l both_file) <(./myls -l@ both_file))
check_diff "$diff_output" "combined attributes listing"

# Test multiple files with different attributes
echo -e "\n${BLUE}Testing mixed files listing${RESET}"
echo "Testing ls -l with all files:"
diff_output=$(diff <(ls -l regular_file acl_file xattr_file both_file) \
                   <(./myls -l@ regular_file acl_file xattr_file both_file))
check_diff "$diff_output" "mixed files listing"

# Test directory containing files with different attributes
echo -e "\n${BLUE}Testing directory listing${RESET}"
mv regular_file acl_file xattr_file both_file regular_dir/
echo "Testing ls -l regular_dir:"
diff_output=$(diff <(ls -l regular_dir) <(./myls -l regular_dir))
check_diff "$diff_output" "directory with mixed files"

# Print test summary
echo -e "\n${BLUE}Test Summary:${RESET}"
echo -e "Total tests: $total_tests"
echo -e "Successful tests: $successful_tests"
if [ $total_tests -eq $successful_tests ]; then
    echo -e "${GREEN}All tests passed successfully!${RESET}"
else
    echo -e "${RED}Some tests failed ($((total_tests - successful_tests)) failures)${RESET}"
fi

# Cleanup
cleanup() {
    # Remove ACLs and extended attributes before deletion
    setfacl -b regular_dir/* 2>/dev/null
    setfacl -b regular_dir acl_dir 2>/dev/null
    cd - > /dev/null 2>&1
    rm -rf /tmp/ls_test_acl
}

trap cleanup EXIT

echo -e "\n${GREEN}Tests completed!${RESET}"