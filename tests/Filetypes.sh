#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
RESET=$(tput sgr0)

echo -e "\n${RED}Filetypes.sh${RESET}\n"

# Get the directory where the script is located
SCRIPT_DIR=$(dirname "$(readlink -f "$0")")
PROJECT_DIR=$(dirname "$SCRIPT_DIR")

# Create temporary directory
mkdir /tmp/ls_test
ln -s "$PROJECT_DIR/ft_ls" /tmp/ls_test/myls
cd /tmp/ls_test

# Create a named pipe (FIFO)
mkfifo testpipe

# Create a character special file (needs root)
#sudo mknod testchar c 1 3    # Major number 1, minor number 3 for null device

# Create a block special file (needs root)
#$sudo mknod testblock b 8 0   # Major number 8, minor number 0 for first SCSI disk

# Create a Unix domain socket
python3 -c "import socket; s = socket.socket(socket.AF_UNIX); s.bind('testsock')"

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