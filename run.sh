#!/bin/bash

# Compile C++ app
# Use standart C++ 2017 '-std=c++17'
g++ -std=c++17 -I includes -L libs src/*.cpp -o bin/app -lpng

# Run app
./bin/app
