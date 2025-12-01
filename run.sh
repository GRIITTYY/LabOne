#!/bin/bash

# This script builds and runs the LabOne C project, hiding all build output.
echo ""

# 1. Configure the project
cmake -S . -B build &> /dev/null

# 2. Build the project
cmake --build build &> /dev/null

# 3. Run the final executable
./build/LabOne