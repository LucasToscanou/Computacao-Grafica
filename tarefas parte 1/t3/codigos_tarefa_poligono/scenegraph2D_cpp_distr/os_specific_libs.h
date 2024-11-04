#ifndef OS_SPECIFIC_LIBS_H
#define OS_SPECIFIC_LIBS_H

#ifdef _WIN32
    #include <windows.h>
    #include <glad/glad.h>
#elif defined(__APPLE__)
    #include <OpenGL/gl3.h>
#elif defined(__linux__)
    // #include <GL/glew.h>
    #include "include/glad/glad.h"
    #include <GL/gl.h>
#endif
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#endif // OS_SPECIFIC_LIBS_H
