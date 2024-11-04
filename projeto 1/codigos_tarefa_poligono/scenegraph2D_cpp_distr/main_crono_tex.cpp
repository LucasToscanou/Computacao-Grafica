#include "os_specific_libs.h"
#include <GLFW/glfw3.h>
#include "scene.h"
#include "state.h"
#include "camera2d.h"
#include "color.h"
#include "texture.h"
#include "transform.h"
#include "error.h"
#include "shader.h"
#include "disk.h"
#include "triangle.h"

#include <iostream>

static ScenePtr scene;
static CameraPtr camera;
static DiskPtr diskEarth;
static DiskPtr diskSun;

class MovePointer;
using MovePointerPtr = std::shared_ptr<MovePointer>;
class MovePointer : public Engine 
{
  TransformPtr m_trf;
  float angle;
  float speed;
  float radius;
protected:
  MovePointer(TransformPtr trf, float speed, float radius) 
  : m_trf(trf), angle(0.0f), speed(speed), radius(radius)
  {
  }
public:
  static MovePointerPtr Make(TransformPtr trf, float speed, float radius)
  {
    return MovePointerPtr(new MovePointer(trf, speed, radius));
  }

  virtual void Update(float dt)
  {
    // Update the angle of rotation based on the speed
    angle += dt * speed;

    // Ensure the angle stays within [0, 360) degrees
    if (angle > 360.0f)
      angle -= 360.0f;

    // Calculate new position using circular motion
    float x = radius * cos(glm::radians(angle));
    float y = radius * sin(glm::radians(angle));

    // Reset transformation and apply translation for circular motion
    m_trf->LoadIdentity();
    m_trf->Translate(x, y, 0);
  }
};

static void initialize (void)
{
  // set background color: white 
  glClearColor(1.0f,1.0f,1.0f,1.0f);
  // enable depth test 
  glEnable(GL_DEPTH_TEST);

  // create objects
  camera = Camera2D::Make(-10,10,-10,10);

  // // Load and create a background texture
  // auto texBackground = Texture::Make("face", "../images/space.jpg");

  // // Create a full-screen quad for the background
  // auto trfBackground = Transform::Make();
  // trfBackground->Scale(20.0f, 20.0f, 1.0f); // Make sure it covers the entire screen
  // auto background = Node::Make(trfBackground, {texBackground}, {Quad::Make()}); // Assuming Quad::Make() creates a textured quad


  // Disk common parameters
  int nslice = 128;
  int radius = 1.0f;

  // // Background
  // auto trfBackground = Transform::Make();
  // auto texBackground = Texture::Make("face","./images/space.jpg");
  // auto background = Node::Make(trfBackground,{Color::Make(1.0f,1.0f,1.0f),texBackground},{Disk::Make(nslice, 20.0f)});

  // Earth
  auto trfEarth = Transform::Make();
  trfEarth->Translate(3,3,0);
  auto texEarth = Texture::Make("face","./images/earth.jpg");
  auto earth = Node::Make(trfEarth,{Color::Make(0.0f,0.5f,0.8f),texEarth},{Disk::Make(nslice, radius)});

  // Moon
  auto trfMoon = Transform::Make();
  trfMoon->Translate(0.1f,0.1f,0);
  auto texMoon = Texture::Make("face","./images/moon.jpg");
  auto moon = Node::Make(trfMoon,{Color::Make(0.8f, 0.8f, 0.8f),texMoon},{Disk::Make(nslice, 0.5*radius)});

  // Sun
  auto trfSun = Transform::Make();
  auto texSun = Texture::Make ("face","./images/sun.jpg");
  auto sun = Node::Make(trfSun,{Color::Make(1.0f, 0.9f, 0.0f),texSun},{Disk::Make(nslice, 2.0*radius)});
  

  // Venus
  auto trfVenus = Transform::Make();
  auto texVenus = Texture::Make ("face","./images/venus.jpg");
  auto venus = Node::Make(trfVenus,{Color::Make(1.0f, 0.9f, 0.0f),texVenus},{Disk::Make(nslice, 0.5*radius)});

  // Add nodes to the scene
  earth->AddNode(moon);
  sun->AddNode(earth);
  sun->AddNode(venus);
  // background->AddNode(sun);

  auto shader = Shader::Make();
  shader->AttachVertexShader("../shaders/vertex.glsl");
  shader->AttachFragmentShader("../shaders/fragment.glsl");
  shader->Link();
  
  // Create the root node and the scene
  auto root = Node::Make(shader, {sun});
  scene = Scene::Make(root);

  scene->AddEngine(MovePointer::Make(trfEarth, 20.0f, 7.0f));
  scene->AddEngine(MovePointer::Make(trfMoon, 80.0f, 2.0f));
  scene->AddEngine(MovePointer::Make(trfVenus, 50.0f, 4.0f));
}

static void display (GLFWwindow* win)
{ 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear window 
  
  Error::Check("before render");
  scene->Render(camera);
  Error::Check("after render");
}

static void error (int code, const char* msg)
{
  printf("GLFW error %d: %s\n", code, msg);
  glfwTerminate();
  exit(0);
}

static void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
  if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void resize (GLFWwindow* win, int width, int height)
{
  glViewport(0,0,width,height);
}

static void update (float dt)
{
  scene->Update(dt);
}

int main ()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

  glfwSetErrorCallback(error);

  GLFWwindow* win = glfwCreateWindow(600,400,"Mini Sistema Solar",nullptr,nullptr);
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
  printf("OpenGL version: %s\n", glGetString(GL_VERSION));

  glfwSetFramebufferSizeCallback(win, resize);  // resize callback
  glfwSetKeyCallback(win, keyboard);            // keyboard callback
  
  initialize();

  float t0 = glfwGetTime();
  while(!glfwWindowShouldClose(win)) {
    float t = glfwGetTime();
    update(t-t0);
    t0 = t;
    display(win);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

