// Draw a triangle
// Simple example to draw a colored triangle

#include "os_specific_libs.h"

#include "error.h"
#include "shader.h"
#include "triangle.h"
#include "polygon.h"

// static TrianglePtr tri;
static PolygonPtr poly;
static ShaderPtr shd;

static void initialize()
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  float coord[] = {
    0.0f,  0.5f,   // Top vertex
   -0.47f, 0.15f,  // Upper left
   -0.29f, -0.4f,  // Lower left
    0.29f, -0.4f,  // Lower right
    0.47f, 0.15f   // Upper right
  };

  unsigned char color[] = {
    255, 0, 0,    // Red for top vertex
    0, 255, 0,    // Green for upper left
    0, 0, 255,    // Blue for lower left
    255, 255, 0,  // Yellow for lower right
    255, 0, 255   // Magenta for upper right
  };

  unsigned int index[] = { 0, 1, 2, 3, 4 };


  shd = Shader::Make();
  shd->AttachVertexShader("../shaders/vertex.glsl");
  std::cout << "main.cpp: got here -  initialize()" << std::endl;
  shd->AttachFragmentShader("../shaders/fragment.glsl");
  shd->Link();
  poly = Polygon::Make(coord, color, index, 5); //sizeof(index)/sizeof(index[0]
  Error::Check("initialize");
}

static void display(GLFWwindow * win)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  shd->UseProgram();
  // tri->Draw();
  poly->Draw();
  Error::Check("display");
}

static void keyboard(GLFWwindow * window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void resize(GLFWwindow * win, int width, int height)
{
  glViewport(0, 0, width, height);
}

static void error (int code, const char* msg)
{
  printf("GLFW error %d: %s\n", code, msg);
  glfwTerminate();
  exit(0);
}

int main()
{
  glfwSetErrorCallback(error);
  if (glfwInit() != GLFW_TRUE) {
      std::cerr << "Could not initialize GLFW" << std::endl;
      return 0;
    }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);     // required for mac os
#if __APPLE__
  glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);  // option for mac os
#endif

  
  GLFWwindow* win = glfwCreateWindow(600, 400, "Polygon", nullptr, nullptr);
  if (!win) {
      std::cerr << "Could not create GLFW window" << std::endl;
      return 0;
    }
  glfwMakeContextCurrent(win);

#ifdef __glad_h_
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      printf("Failed to initialize GLAD OpenGL context\n");
      exit(1);
    }
#endif

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  glfwSetFramebufferSizeCallback(win, resize);  // resize callback
  glfwSetKeyCallback(win, keyboard);   // keyboard callback

  initialize();
  while (!glfwWindowShouldClose(win)) {
    display(win);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

