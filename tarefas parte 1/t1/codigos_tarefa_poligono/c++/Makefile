# Target executable
TARGET = t

# Default target
all: $(TARGET)

# Include paths
INC = -I. -Iinclude -Iinclude/glad

# Library paths
LIB = -lglfw -lGLEW -lGL -ldl

# Compiler and flags
CPP = g++
CC = gcc
CPPFLAGS = -g -Wall -std=c++17 -DGL_SILENCE_DEPRECATION
CFLAGS = -g -Wall -DGL_SILENCE_DEPRECATION

# Source files
SRC = \
  main.cpp \
  error.cpp \
  shader.cpp \
  triangle.cpp \
  polygon.cpp \
  src/glad.c   # Ensure glad.c is in the correct location

# Object files
OBJ = $(addsuffix .o, $(basename $(SRC)))

# Compile rule for .cpp files
%.cpp.o:
	$(CPP) $(INC) $(CPPFLAGS) -c $< -o $@

# Compile rule for .c files
%.c.o:
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

# Link rule
$(TARGET): $(OBJ)
	$(CPP) $(OBJ) $(LIB) -o $@

# Clean rule
clean:
	rm -f $(TARGET) $(OBJ)
