#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
RESET=$(tput sgr0)

echo -e "\n${RED}Recursive and Time Sorting Test${RESET}\n"

# Get the directory where the script is located
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
PROJECT_DIR=$(dirname "$SCRIPT_DIR")

# Create temporary directory
mkdir -p /tmp/ls_test_rt
cp ../myls /tmp/ls_test_rt/myls > /dev/null 2>&1
chmod +x /tmp/ls_test_rt/myls > /dev/null 2>&1
cd /tmp/ls_test_rt

# Initialize counters
total_tests=0
successful_tests=0

# Create test environment with time differences
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

# Test ls -r
echo -e "\n${BLUE}Testing ls -r${RESET}"
echo "Real ls -r output:"
ls -r
echo -e "\nMy ls -r output:"
./myls -r

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -r) <(./myls -r))
check_diff "$diff_output" "ls -r"

# Test ls -t
echo -e "\n${BLUE}Testing ls -t${RESET}"
echo "Real ls -t output:"
ls -t
echo -e "\nMy ls -t output:"
./myls -t

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -t) <(./myls -t))
check_diff "$diff_output" "ls -t"

# Test ls -r with multiple parameters
echo -e "\n${BLUE}Testing ls -r with multiple parameters${RESET}"
echo "Real ls -r dir1 dir2 dir3 output:"
ls -r dir1 dir2 dir3
echo -e "\nMy ls -r dir1 dir2 dir3 output:"
./myls -r dir1 dir2 dir3

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -r dir1 dir2 dir3) <(./myls -r dir1 dir2 dir3))
check_diff "$diff_output" "ls -r with multiple parameters"

# Test ls -t with multiple parameters
echo -e "\n${BLUE}Testing ls -t with multiple parameters${RESET}"
echo "Real ls -t dir1 dir2 dir3 output:"
ls -t dir1 dir2 dir3
echo -e "\nMy ls -t dir1 dir2 dir3 output:"
./myls -t dir1 dir2 dir3

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -t dir1 dir2 dir3) <(./myls -t dir1 dir2 dir3))
check_diff "$diff_output" "ls -t with multiple parameters"

# Test ls -r with nested directories
echo -e "\n${BLUE}Testing ls -r with nested directories${RESET}"
echo "Real ls -r dir1/subdir1 dir2/subdir1 output:"
ls -r dir1/subdir1 dir2/subdir1
echo -e "\nMy ls -r dir1/subdir1 dir2/subdir1 output:"
./myls -r dir1/subdir1 dir2/subdir1

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -r dir1/subdir1 dir2/subdir1) <(./myls -r dir1/subdir1 dir2/subdir1))
check_diff "$diff_output" "ls -r with nested directories"

# Test ls -t with nested directories
echo -e "\n${BLUE}Testing ls -t with nested directories${RESET}"
echo "Real ls -t dir1/subdir1 dir2/subdir1 output:"
ls -t dir1/subdir1 dir2/subdir1
echo -e "\nMy ls -t dir1/subdir1 dir2/subdir1 output:"
./myls -t dir1/subdir1 dir2/subdir1

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -t dir1/subdir1 dir2/subdir1) <(./myls -t dir1/subdir1 dir2/subdir1))
check_diff "$diff_output" "ls -t with nested directories"

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
    cd - > /dev/null 2>&1
    rm -rf /tmp/ls_test_rt
}

trap cleanup EXIT

echo -e "\n${GREEN}Tests completed!${RESET}"