#!/bin/bash

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
ASAN_OPTIONS=detect_leaks=0 ./myls -l
ls -l

# Clean up
rm testpipe testsock myls #testchar testblock 
cd -
rmdir /tmp/ls_test