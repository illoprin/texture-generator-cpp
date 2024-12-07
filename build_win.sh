#!/bin/bash


# This shit is not working fuck shit говно
x86_64-w64-mingw32-g++ -std=c++17  -I /home/ilya/.local/include/libpng16 -I includes src/*.cpp -o bin/app.exe -Wl, -Bstatic -L /home/ilya/.local/lib/libpng16 -lpng16 -Wl, -Bdynamic