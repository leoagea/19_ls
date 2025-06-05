#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
RESET=$(tput sgr0)

echo -e "\n${RED}Size Format Test (-h flag)${RESET}\n"

# Setup test directory
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
PROJECT_DIR=$(dirname "$SCRIPT_DIR")

# Create temporary directory
mkdir -p /tmp/ls_test_size
cp ../myls /tmp/ls_test_size/myls > /dev/null 2>&1
chmod +x /tmp/ls_test_size/myls > /dev/null 2>&1
cd /tmp/ls_test_size

# Create test files with different sizes
echo -e "\nCreating test files..."

# Empty file (0B)
touch empty_file

# Small file (1K)
dd if=/dev/zero of=small_file bs=1K count=1 2>/dev/null

# Medium file (10M)
dd if=/dev/zero of=medium_file bs=1M count=10 2>/dev/null

# Large file (100M)
dd if=/dev/zero of=large_file bs=1M count=100 2>/dev/null

# Huge file (1.5G)
dd if=/dev/zero of=huge_file bs=1M count=1500 2>/dev/null

# Create symbolic link
ln -s large_file symlink_file

echo -e "\n${BLUE}Testing with real ls:${RESET}"
ls -lh

echo -e "\n${GREEN}Testing with my ls:${RESET}"
echo "My ls -lh output:"
./myls -lh

echo -e "\n${RED}Diff between real ls and my ls:${RESET}"
diff_output=$(diff <(ls -lh) <(./myls -lh))
if [ -z "$diff_output" ]; then
    echo -e "${GREEN}No differences found. My ls works correctly!${RESET}"
else
    echo -e "${RED}Differences found:${RESET}"
    echo "$diff_output"
fi

# Clean up
cleanup() {
    cd - > /dev/null 2>&1
    rm -rf /tmp/ls_test_size
}

trap cleanup EXIT