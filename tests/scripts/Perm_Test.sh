#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
RESET=$(tput sgr0)

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

: '
echo -e "\n${BLUE}Testing individual files with real ls:${RESET}"
ls -l setuid_file
ls -l setgid_file
ls -l sticky_dir
ls -l setuid_setgid_file
ls -l all_bits_dir

echo -e "\n${GREEN}Testing individual files with my ls:${RESET}"
./myls -l setuid_file
./myls -l setgid_file
./myls -l sticky_dir
./myls -l setuid_setgid_file
./myls -l all_bits_dir
'
# Clean up
cleanup() {
    cd - > /dev/null 2>&1
    rm -rf /tmp/ls_test_bits
}

trap cleanup EXIT