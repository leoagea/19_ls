#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
RESET=$(tput sgr0)

# Initialize counters
total_tests=0
successful_tests=0

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

echo -e "\n${RED}Special Bits Test${RESET}\n"

# Get the directory where the script is located
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
PROJECT_DIR=$(dirname "$SCRIPT_DIR")

# Create temporary directory
mkdir -p /tmp/ls_test_bits
cp ../myls /tmp/ls_test_bits/myls > /dev/null 2>&1
chmod +x /tmp/ls_test_bits/myls > /dev/null 2>&1
cd /tmp/ls_test_bits
echo -e "${BLUE}Current directory:${RESET}"
ls -l
pwd

# Create test files and set special bits
echo -e "\nCreating test files..."

# Test setuid (4000)
touch setuid_file
# chmod u+s setuid_file
chmod 4755 setuid_file

touch setuid_file_executable
chmod u+sx setuid_file_executable

touch setuid_file_executable_2
chmod u+s setuid_file_executable_2

# Test setgid (2000)
touch setgid_file
# chmod u+g setgid_file
chmod 2755 setgid_file

# Test sticky bit on directory (1000)
mkdir sticky_dir
chmod 1777 sticky_dir

# Test setuid + setgid combination
touch setuid_setgid_file
chmod 6755 setuid_setgid_file

# Test all special bits on directory
mkdir all_bits_dir
chmod 7777 all_bits_dir

# Create a regular file for comparison
touch regular_file
chmod 644 regular_file

echo -e "\n${BLUE}Testing with real ls:${RESET}"
echo "Regular ls -l output:"
ls -l

echo -e "\n${GREEN}Testing with my ls:${RESET}"
echo "My ls -l output:"
./myls -l

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -l) <(./myls -l))
check_diff "$diff_output" "basic ls -l"

# Replace individual file diff checks with:
echo -e "\n${RED}Testing individual files:${RESET}"
diff_output=$(diff <(ls -l setuid_file) <(./myls -l setuid_file))
check_diff "$diff_output" "setuid_file"

diff_output=$(diff <(ls -l setgid_file) <(./myls -l setgid_file))
check_diff "$diff_output" "setgid_file"

diff_output=$(diff <(ls -l sticky_dir) <(./myls -l sticky_dir))
check_diff "$diff_output" "sticky_dir"

diff_output=$(diff <(ls -l setuid_setgid_file) <(./myls -l setuid_setgid_file))
check_diff "$diff_output" "setuid_setgid_file"

diff_output=$(diff <(ls -l all_bits_dir) <(./myls -l all_bits_dir))
check_diff "$diff_output" "all_bits_dir"

# Add test summary before cleanup
echo -e "\n${BLUE}Test Summary:${RESET}"
echo -e "Total tests: $total_tests"
echo -e "Successful tests: $successful_tests"
if [ $total_tests -eq $successful_tests ]; then
    echo -e "${GREEN}All tests passed successfully!${RESET}"
else
    echo -e "${RED}Some tests failed ($((total_tests - successful_tests)) failures)${RESET}"
fi

# Clean up
cleanup() {
    cd - > /dev/null 2>&1
    rm -rf /tmp/ls_test_bits
}

trap cleanup EXIT