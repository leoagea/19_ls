#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
RESET=$(tput sgr0)

echo -e "\n${RED}OldFile.sh${RESET}\n"

# Get the directory where the script is located
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
PROJECT_DIR=$(dirname "$SCRIPT_DIR")

# Create temporary directory
cd /tmp/ls_test

# Create a temporary file
touch testfile
touch -t 202301010000 oldfile    # Set date to Jan 1, 2023
TZ=CEST touch -d "1 day ago" onedayold    # Set date to 1 day ago
TZ=CEST touch -d "1 month ago" onemonthold    # Set date to 1 month ago
TZ=CEST touch -d "3 months ago" threemonthsold    # Set date to 3 months ago
TZ=CEST touch -d "5 months ago" fivemonthsold    # Set date to 5 months ago
TZ=CEST touch -d "6 months ago" sixmonthsold    # Set date to 6 months ago
TZ=CEST touch -d "1 year ago" oneyearold    # Set date to 1 year ago
touch -t 204001010000 futurefile    # Set date to Jan 1, 2040

# Test with ls
echo -e "${BLUE}Testing with real ls:${RESET}"
ls -l
echo -e "${GREEN}Testing with my ls:${RESET}"
ASAN_OPTIONS=detect_leaks=0 ./myls -l 

# Clean up
cleanup() {
    cd - > /dev/null 2>&1
    rm -f /tmp/ls_test/*
    rmdir /tmp/ls_test 2>/dev/null
}
trap cleanup EXIT

