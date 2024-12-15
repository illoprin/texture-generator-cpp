
CC=g++
CFLAGS=-std=c++17
TARGET=src/*.cpp
LIB_PATH=libs
INC_PATH=includes
LIBPNG_LIB=-lpng
EXEC_FILE=bin/app
AFTER=./bin/app

all:
	$(CC) $(CFLAGS) $(TARGET) -o $(EXEC_FILE) -I $(INC_PATH) -L $(LIB_PATH) $(LIBPNG_LIB) && $(AFTER)
