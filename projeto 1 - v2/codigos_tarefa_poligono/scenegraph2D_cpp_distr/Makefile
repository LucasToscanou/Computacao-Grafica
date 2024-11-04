TARGET = t

all: $(TARGET)

INC = -I. -Iinclude -Iinclude/glad
LIB = -lglfw -lGLEW -lGL -ldl

CPP = g++
# CPPFLAGS = -O2 -Wall -std=c++17 -DGL_SILENCE_DEPRECATION 
CPPFLAGS = -g -Wall -std=c++17 -DGL_SILENCE_DEPRECATION 

SRC = \
  camera2d.cpp \
  color.cpp \
  disk.cpp \
  error.cpp \
  node.cpp \
  scene.cpp \
  shader.cpp \
  state.cpp \
  texture.cpp \
  transform.cpp \
  triangle.cpp \
  square.cpp \
  main_crono_tex.cpp \
  image.cpp \
  src/glad.c \
  

OBJ = $(addsuffix .o, $(basename $(SRC))) 

.cpp.o:
	$(CPP) $(INC) $(CPPFLAGS) -c $< -o $@ 

$(TARGET): $(OBJ) Makefile 
	$(CPP) $(LIB) -o $@ $(OBJ) -lstdc++ -lglfw -lassimp

clean:
	rm -f $(TARGET) $(OBJ)
