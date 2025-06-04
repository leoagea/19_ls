#!/bin/bash

RED=$(tput setaf 1)
GREEN=$(tput setaf 2)
BLUE=$(tput setaf 4)
RESET=$(tput sgr0)

echo "# ft_ls path" >> ~/.bashrc
echo "export PATH=\$$PATH:$(PWD)" >> ~/.bashrc
echo "${GREEN}Setup complete! To use ft_ls from anywhere, either:${NC}"
echo "1. Run: source ~/.bashrc"
echo "2. Or restart your terminal"