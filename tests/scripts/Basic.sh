#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
RESET=$(tput sgr0)

echo -e "\n${RED}Basic Features Test${RESET}\n"

# Get the directory where the script is located
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
PROJECT_DIR=$(dirname "$SCRIPT_DIR")

# Create temporary directory
mkdir -p /tmp/ls_test_basic
cp ../myls /tmp/ls_test_basic/myls > /dev/null 2>&1
chmod +x /tmp/ls_test_basic/myls > /dev/null 2>&1
cd /tmp/ls_test_basic

# Create test environment
echo -e "\n${BLUE}Creating test environment...${RESET}"

# Create some regular files
touch file1.txt
touch file2.txt
echo "Hello" > file3.txt

# Create hidden files
touch .hidden1
touch .hidden2

# Create directories
mkdir test_dir
mkdir .hidden_dir

# Create symbolic links
ln -s file1.txt symlink1
ln -s test_dir dir_symlink
ln -s nonexistent_file broken_link

echo -e "\n${BLUE}Testing ls without arguments${RESET}"
echo "Real ls output:"
ls
echo -e "\nMy ls output:"
./myls

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls) <(./myls))
if [ -z "$diff_output" ]; then
    echo -e "${GREEN}No differences found. My ls works correctly!${RESET}"
else
    echo -e "${RED}Differences found:${RESET}"
    echo "$diff_output"
fi

echo -e "\n${BLUE}Testing ls -a${RESET}"
echo "Real ls -a output:"
ls -a
echo -e "\nMy ls -a output:"
./myls -a

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -a) <(./myls -a))
if [ -z "$diff_output" ]; then
    echo -e "${GREEN}No differences found. My ls works correctly!${RESET}"
else
    echo -e "${RED}Differences found:${RESET}"
    echo "$diff_output"
fi

echo -e "\n${BLUE}Testing ls -l${RESET}"
echo "Real ls -l output:"
ls -l
echo -e "\nMy ls -l output:"
./myls -l

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -l) <(./myls -l))
if [ -z "$diff_output" ]; then
    echo -e "${GREEN}No differences found. My ls works correctly!${RESET}"
else
    echo -e "${RED}Differences found:${RESET}"
    echo "$diff_output"
fi

echo -e "\n${BLUE}Testing symbolic links${RESET}"
echo "Real ls -l symlink1:"
ls -l symlink1
echo -e "\nMy ls -l symlink1:"
./myls -l symlink1

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -l symlink1) <(./myls -l symlink1))
if [ -z "$diff_output" ]; then
    echo -e "${GREEN}No differences found for symlink1. My ls works correctly!${RESET}"
else
    echo -e "${RED}Differences found for symlink1:${RESET}"
    echo "$diff_output"
fi

echo -e "\nDirectory symlink test:"
echo "Real ls -l dir_symlink:"
ls -l dir_symlink
echo -e "\nMy ls -l dir_symlink:"
./myls -l dir_symlink

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -l dir_symlink) <(./myls -l dir_symlink))
if [ -z "$diff_output" ]; then
    echo -e "${GREEN}No differences found for dir_symlink. My ls works correctly!${RESET}"
else
    echo -e "${RED}Differences found for dir_symlink:${RESET}"
    echo "$diff_output"
fi

echo -e "\nBroken link test:"
echo "Real ls -l broken_link:"
ls -l broken_link
echo -e "\nMy ls -l broken_link:"
./myls -l broken_link

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -l broken_link) <(./myls -l broken_link))
if [ -z "$diff_output" ]; then
    echo -e "${GREEN}No differences found for broken_link. My ls works correctly!${RESET}"
else
    echo -e "${RED}Differences found for broken_link:${RESET}"
    echo "$diff_output"
fi

# Initialize counter for successful tests
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

# Replace all existing diff checks with:
diff_output=$(diff <(ls) <(./myls))
check_diff "$diff_output" "basic ls"

diff_output=$(diff <(ls -a) <(./myls -a))
check_diff "$diff_output" "ls -a"

diff_output=$(diff <(ls -l) <(./myls -l))
check_diff "$diff_output" "ls -l"

diff_output=$(diff <(ls -l symlink1) <(./myls -l symlink1))
check_diff "$diff_output" "symlink1"

diff_output=$(diff <(ls -l dir_symlink) <(./myls -l dir_symlink))
check_diff "$diff_output" "dir_symlink"

diff_output=$(diff <(ls -l broken_link) <(./myls -l broken_link))
check_diff "$diff_output" "broken_link"

# Add before the cleanup function:
echo -e "\n${BLUE}Test Summary:${RESET}"
echo -e "Total tests: $total_tests"
echo -e "Successful tests: $successful_tests"
if [ $total_tests -eq $successful_tests ]; then
    echo -e "${GREEN}All tests passed successfully!${RESET}"
else
    echo -e "${RED}Some tests failed ($((total_tests - successful_tests)) failures)${RESET}"
fi

# cleanup function remains the same
cleanup() {
    cd - > /dev/null 2>&1
    rm -rf /tmp/ls_test_basic
}

trap cleanup EXIT

echo -e "\n${GREEN}Tests completed!${RESET}"