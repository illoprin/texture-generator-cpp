#!/bin/bash

g++ -I includes -L libs src/*.cpp -lpng -o bin/app && ./bin/app