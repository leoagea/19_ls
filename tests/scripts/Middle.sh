#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
RESET=$(tput sgr0)

echo -e "\n${RED}Recursive Listing and Multiple Options Test${RESET}\n"

# Initialize counters
total_tests=0
successful_tests=0

# Get the directory where the script is located
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
PROJECT_DIR=$(dirname "$SCRIPT_DIR")

# Create temporary directory
mkdir -p /tmp/ls_test_recursive
cp ../myls /tmp/ls_test_recursive/myls > /dev/null 2>&1
chmod +x /tmp/ls_test_recursive/myls > /dev/null 2>&1
cd /tmp/ls_test_recursive

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

# Test recursive listing (-R)
echo -e "\n${BLUE}Testing ls -R${RESET}"
echo "Real ls -R output:"
ls -R
echo -e "\nMy ls -R output:"
./myls -R

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -R) <(./myls -R))
check_diff "$diff_output" "ls -R"

# Test multiple options as separate arguments
echo -e "\n${BLUE}Testing multiple options as separate arguments${RESET}"
echo "Testing -l -t:"
diff_output=$(diff <(ls -l -t) <(./myls -l -t))
check_diff "$diff_output" "ls -l -t (separate)"

echo "Testing -l -a:"
diff_output=$(diff <(ls -l -a) <(./myls -l -a))
check_diff "$diff_output" "ls -l -a (separate)"

echo "Testing -l -R:"
diff_output=$(diff <(ls -l -R) <(./myls -l -R))
check_diff "$diff_output" "ls -l -R (separate)"

# Test combined options
echo -e "\n${BLUE}Testing combined options${RESET}"
echo "Testing -lt:"
diff_output=$(diff <(ls -lt) <(./myls -lt))
check_diff "$diff_output" "ls -lt (combined)"

echo "Testing -la:"
diff_output=$(diff <(ls -la) <(./myls -la))
check_diff "$diff_output" "ls -la (combined)"

echo "Testing -lR:"
diff_output=$(diff <(ls -lR) <(./myls -lR))
check_diff "$diff_output" "ls -lR (combined)"

# Test complex combinations
echo -e "\n${BLUE}Testing complex option combinations${RESET}"
echo "Testing -laR:"
diff_output=$(diff <(ls -laR) <(./myls -laR))
check_diff "$diff_output" "ls -laR"

echo "Testing -l -a -R:"
diff_output=$(diff <(ls -l -a -R) <(./myls -l -a -R))
check_diff "$diff_output" "ls -l -a -R (separate)"

# Test with specific paths
echo -e "\n${BLUE}Testing options with specific paths${RESET}"
echo "Testing -R dir1 dir2:"
diff_output=$(diff <(ls -R dir1 dir2) <(./myls -R dir1 dir2))
check_diff "$diff_output" "ls -R with paths"

echo "Testing -lt dir1:"
diff_output=$(diff <(ls -lt dir1) <(./myls -lt dir1))
check_diff "$diff_output" "ls -lt with path"

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
    rm -rf /tmp/ls_test_recursive
}

trap cleanup EXIT

echo -e "\n${GREEN}Tests completed!${RESET}"