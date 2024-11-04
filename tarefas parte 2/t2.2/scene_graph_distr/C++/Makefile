
TARGET = t

all: $(TARGET)

INC = -I. -Iinclude -Iinclude/glad
LIB = -lglfw -lGL -ldl

CPP = g++
# CPPFLAGS = -O2 -Wall -std=c++17 -DGL_SILENCE_DEPRECATION 
CPPFLAGS = -g -Wall -std=c++17 -DGL_SILENCE_DEPRECATION 


SRC = \
  arcball.cpp \
  camera2d.cpp \
  camera3d.cpp \
  color.cpp \
  cube.cpp \
  error.cpp \
  framebuffer.cpp \
  grid.cpp \
  image.cpp \
  light.cpp \
  material.cpp \
  mesh.cpp \
  node.cpp \
  quad.cpp \
  polyoffset.cpp \
  scene.cpp \
  shader.cpp \
  sphere.cpp \
  state.cpp \
  texcube.cpp \
  texdepth.cpp \
  texture.cpp \
  transform.cpp \
  triangle.cpp \
  cylinder.cpp \
  main_3d.cpp \
  src/glad.c \

OBJ = $(addsuffix .o, $(basename $(SRC))) 

.cpp.o:
	$(CPP) $(INC) $(CPPFLAGS) -c $< -o $@ 

$(TARGET): $(OBJ) Makefile 
	$(CPP) $(LIB) -o $@ $(OBJ) -lstdc++ -lglfw -lassimp

clean:
	rm -f $(TARGET) $(OBJ)
