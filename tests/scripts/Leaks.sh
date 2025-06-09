#!/bin/bash

# Color definitions
RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
YELLOW=$(tput setaf 3)
RESET=$(tput sgr0)

echo -e "\n${BLUE}=== Testing ls with Valgrind ===${RESET}\n"

# Test environment setup
TEST_DIR="/tmp/ls_test_valgrind"
mkdir -p "$TEST_DIR"
cp ../myls "$TEST_DIR/myls"
cd "$TEST_DIR"

# Test cases with different flag combinations
TESTS=(
    # Basic flags
    "-l"
    "-a"
    "-R"
    # Multiple flags combinations
    "-la"
    "-lR"
    "-laR"
    "-lRa"
    # Time and size sorting
    "-lt"
    "-lS"
    "-ltr"
    "-lSr"
    # Complex combinations
    "-laRt"
    "-laRS"
    # Special cases
    "-la /etc"
    "-lR /usr/include"
    "-laR /proc"
    # Edge cases
    "nonexistent"
    "-l /root"
    "-laR . /etc /tmp"
)

# Create test environment
setup_test_env() {
    echo -e "${BLUE}Creating test environment...${RESET}"
    
    # Create various types of files
    mkdir -p dir1/subdir1/subsubdir
    mkdir -p dir2/.hidden_dir
    touch regular_file
    touch .hidden_file
    ln -s regular_file symlink
    mkfifo testpipe
    echo "content" > file_with_content
    dd if=/dev/zero of=large_file bs=1M count=10 2>/dev/null
    
    # Set different timestamps
    touch -t 202001010000 old_file
    touch -t 202312312359 new_file
}

# Run single test with valgrind
run_test() {
    local args=$1
    echo -e "\n${YELLOW}Testing: ./myls $args${RESET}"
    
    valgrind --leak-check=full \
             --show-leak-kinds=all \
             --track-origins=yes \
             --suppressions=valgrind.supp \
             --error-exitcode=1 \
             ./myls $args 2>&1 | grep -A 5 "LEAK SUMMARY"
    
    local status=${PIPESTATUS[0]}
    if [ $status -eq 0 ] || [ $status -eq 2 ]; then
        echo -e "${GREEN}✓ No memory leaks detected${RESET}"
    else
        echo -e "${RED}✗ Memory leaks detected (exit code: $status)${RESET}"
    fi
}

# Create suppression file
cat > valgrind.supp << 'EOF'
{
    getpwuid_leak
    Memcheck:Leak
    match-leak-kinds: reachable
    ...
    fun:getpwuid
    ...
}
{
    getgrgid_leak
    Memcheck:Leak
    match-leak-kinds: reachable
    ...
    fun:getgrgid
    ...
}
{
    setlocale_leak
    Memcheck:Leak
    match-leak-kinds: reachable
    ...
    fun:setlocale
    ...
}
EOF

# Main execution
main() {
    setup_test_env
    
    # Run all tests
    for test in "${TESTS[@]}"; do
        run_test "$test"
    done
    
    echo -e "\n${BLUE}=== Testing Complete ===${RESET}"
}

# Cleanup function
cleanup() {
    cd - > /dev/null 2>&1
    rm -rf "$TEST_DIR"
}

# Set cleanup trap
trap cleanup EXIT

# Run tests
main