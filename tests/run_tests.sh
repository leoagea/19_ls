#!/bin/bash

GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo "${GREEN}Building ft_ls...${NC}"
cd ..
make
cp ft_ls tests/myls
if [ $? -ne 0 ]; then
    echo "${RED}Build failed${NC}"
    exit 1
fi

echo "${GREEN}Building Docker image...${NC}"
cd tests
docker build -t ls-tests .
if [ $? -ne 0 ]; then
    echo "${RED}Docker build failed${NC}"
    exit 1
fi

echo "${GREEN}Running tests in container...${NC}"
docker run -it ls-tests