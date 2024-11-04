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
#include "square.h"

#include <iostream>

static ScenePtr scene;
static CameraPtr camera;
static float screenWidth, screenHeight;

class MovePointer;
using MovePointerPtr = std::shared_ptr<MovePointer>;
class MovePointer : public Engine 
{
  TransformPtr m_trf;
  float trans_angle;
  float rot_angle;
  float trans_speed;
  float tans_radius;
  float rotation_speed;
protected:
  MovePointer(TransformPtr trf, float trans_speed, float tans_radius, float rotation_speed) 
  : m_trf(trf), trans_angle(0.0f), trans_speed(trans_speed), tans_radius(tans_radius), rotation_speed(rotation_speed)
  {
  }
public:
  static MovePointerPtr Make(TransformPtr trf, float trans_speed, float tans_radius, float rotation_speed)
  {
    return MovePointerPtr(new MovePointer(trf, trans_speed, tans_radius, rotation_speed));
  }

  virtual void Update(float dt)
  {
    // Update the trans_angle of rotation based on the trans_speed
    trans_angle += dt * trans_speed;
    rot_angle += dt * rotation_speed;

    // Ensure the trans_angle stays within [0, 360) degrees
    if (trans_angle > 360.0f)
      trans_angle -= 360.0f;

    if(rot_angle > 360.0f)
      rot_angle -= 360.0f;

    // Calculate new position using circular motion
    float x = tans_radius * cos(glm::radians(trans_angle));
    float y = tans_radius * sin(glm::radians(trans_angle));


    // Reset transformation and apply translation for circular motion
    m_trf->LoadIdentity();
    m_trf->Translate(x, y, 0);
    m_trf->Rotate(rot_angle, 0, 0, 1);
  }
};

static void initialize (void)
{
  // set background color: white 
  glClearColor(1.0f,1.0f,1.0f,1.0f);
  // enable depth test 
  glEnable(GL_DEPTH_TEST);

  // create objects
  float dim = 10.0f;
  camera = Camera2D::Make(-dim,dim,-dim, dim);

  int nslice = 64;
  int radius = 1.0f;

  // Background
  auto bgTrf = Transform::Make();
  bgTrf->Translate(0.0f, 0.0f, -0.1f);
  auto texBg = Texture::Make("face","./images/space.jpg");
  auto bgNode = Node::Make(bgTrf, {Color::Make(0.0f, 0.5f, 0.0f), texBg}, {Square::Make(50, 20)});


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
  auto venus = Node::Make(trfVenus,{Color::Make(1.0f, 0.2f, 0.0f),texVenus},{Disk::Make(nslice, 0.5*radius)});

  // Add nodes to the scene
  earth->AddNode(moon);
  sun->AddNode(earth);
  sun->AddNode(venus);

  auto shader = Shader::Make();
  shader->AttachVertexShader("../shaders/vertex.glsl");
  shader->AttachFragmentShader("../shaders/fragment.glsl");
  shader->Link();
  
  // Create the root node and the scene
  auto root = Node::Make(shader, {bgNode, sun});
  scene = Scene::Make(root);



  scene->AddEngine(MovePointer::Make(trfEarth, 20.0f, 7.0f, 100.0f));
  scene->AddEngine(MovePointer::Make(trfMoon, 50.0f, 2.0f, 0.0f));
  scene->AddEngine(MovePointer::Make(trfVenus, 50.0f, 4.0f, 0.0f));
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

static void resize(GLFWwindow* win, int width, int height)
{
    // Update the viewport to match the new window size
    glViewport(0, 0, width, height);

    // Store the new screen dimensions
    screenWidth = width;
    screenHeight = height;

    // Adjust the camera to match the new aspect ratio
    float aspect = (float)width / (float)height;
    camera = Camera2D::Make(-aspect * 10.0f, aspect * 10.0f, -10.0f, 10.0f);
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

  GLFWwindow* win = glfwCreateWindow(735,400,"Mini Sistema Solar",nullptr,nullptr);
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

