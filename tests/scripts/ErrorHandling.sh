#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
RESET=$(tput sgr0)

echo -e "\n${RED}Error Handling Test${RESET}\n"

# Initialize counters
total_tests=0
successful_tests=0

# Get the directory where the script is located
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
PROJECT_DIR=$(dirname "$SCRIPT_DIR")

# Create temporary directory
mkdir -p /tmp/ls_test_errors
cp ../myls /tmp/ls_test_errors/myls > /dev/null 2>&1
chmod +x /tmp/ls_test_errors/myls > /dev/null 2>&1
cd /tmp/ls_test_errors

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

# Create test environment
echo -e "\n${BLUE}Creating test environment...${RESET}"

# Create regular file and directory
mkdir accessible_dir
echo "content" > accessible_file

# Create inaccessible file and directory
mkdir no_access_dir
echo "secret" > no_access_file
chmod 000 no_access_dir
chmod 000 no_access_file

# Test nonexistent files/folders
echo -e "\n${BLUE}Testing nonexistent files/folders${RESET}"
echo "Testing nonexistent file:"
diff_output=$(diff <(ls nonexistent_file 2>&1) <(./myls nonexistent_file 2>&1))
check_diff "$diff_output" "nonexistent file"

echo "Testing nonexistent directory:"
diff_output=$(diff <(ls nonexistent_dir 2>&1) <(./myls nonexistent_dir 2>&1))
check_diff "$diff_output" "nonexistent directory"

echo "Testing mix of existent and nonexistent:"
diff_output=$(diff <(ls accessible_file nonexistent_file 2>&1) <(./myls accessible_file nonexistent_file 2>&1))
check_diff "$diff_output" "mixed existence"

# Test inaccessible files/folders
echo -e "\n${BLUE}Testing inaccessible files/folders${RESET}"
echo "Testing inaccessible file:"
diff_output=$(diff <(ls no_access_file 2>&1) <(./myls no_access_file 2>&1))
check_diff "$diff_output" "inaccessible file"

echo "Testing inaccessible directory:"
diff_output=$(diff <(ls no_access_dir 2>&1) <(./myls no_access_dir 2>&1))
check_diff "$diff_output" "inaccessible directory"

echo "Testing long format with inaccessible file:"
diff_output=$(diff <(ls -l no_access_file 2>&1) <(./myls -l no_access_file 2>&1))
check_diff "$diff_output" "inaccessible file with -l"

# Test unmanaged/nonexistent options
echo -e "\n${BLUE}Testing unmanaged/nonexistent options${RESET}"
echo "Testing nonexistent option:"
diff_output=$(diff <(ls -z 2>&1) <(./myls -z 2>&1))
check_diff "$diff_output" "nonexistent option"

echo "Testing multiple invalid options:"
diff_output=$(diff <(ls -2yz 2>&1) <(./myls -2yz 2>&1))
check_diff "$diff_output" "multiple invalid options"

echo "Testing mix of valid and invalid options:"
diff_output=$(diff <(ls -la -z 2>&1) <(./myls -la -z 2>&1))
check_diff "$diff_output" "mixed valid/invalid options"

# Test complex error combinations
echo -e "\n${BLUE}Testing complex error combinations${RESET}"
echo "Testing invalid option with nonexistent file:"
diff_output=$(diff <(ls -z nonexistent_file 2>&1) <(./myls -z nonexistent_file 2>&1))
check_diff "$diff_output" "invalid option with nonexistent file"

echo "Testing valid option with inaccessible file:"
diff_output=$(diff <(ls -l no_access_file 2>&1) <(./myls -l no_access_file 2>&1))
check_diff "$diff_output" "valid option with inaccessible file"

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
    # Restore permissions to allow deletion
    chmod 755 no_access_dir
    chmod 644 no_access_file
    cd - > /dev/null 2>&1
    rm -rf /tmp/ls_test_errors
}

trap cleanup EXIT

echo -e "\n${GREEN}Tests completed!${RESET}"