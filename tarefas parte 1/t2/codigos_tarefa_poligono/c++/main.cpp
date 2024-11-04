#include "os_specific_libs.h"

#include "error.h"
#include "shader.h"
#include "triangle.h"
#include "polygon.h"
#include "disk.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>

static DiskPtr diskEarth;
static DiskPtr diskSun;
static ShaderPtr shd;

static void initialize()
{
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  shd = Shader::Make();
  shd->AttachVertexShader("../shaders/vertex.glsl");
  std::cout << "main.cpp: got here -  initialize()" << std::endl;
  shd->AttachFragmentShader("../shaders/fragment.glsl");
  shd->Link();

  diskEarth = Disk::Make(64);
  diskSun = Disk::Make(64);


  Error::Check("initialize");
}


static void display(GLFWwindow * win)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shd->UseProgram();

    float time = (float)glfwGetTime();
    float angle = time * 0.5f; 

    unsigned int transformLoc = glGetUniformLocation(shd->GetProgram(), "M");
    unsigned int colorLoc = glGetUniformLocation(shd->GetProgram(), "objectColor");
    glm::mat4 trans = glm::mat4(1.0f);

    // Earth
    trans = glm::translate(trans, glm::vec3(0.6f, 0.0f, 0.0f));
    trans = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f)) * trans;
    trans = glm::scale(trans, glm::vec3(0.1f, 0.1f, 0.0f));
    glm::vec4 earthColor = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
    glUniform4fv(colorLoc, 1, glm::value_ptr(earthColor));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    diskEarth->Draw();

    // Sun
    trans = glm::mat4(1.0f);
    trans = glm::scale(trans, glm::vec3(0.3f, 0.3f, 0.0f));
    glm::vec4 sunColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
    glUniform4fv(colorLoc, 1, glm::value_ptr(sunColor));
    glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
    diskSun->Draw();

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

  
  GLFWwindow* win = glfwCreateWindow(800, 800, "Earth rotating the sun", nullptr, nullptr);
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

