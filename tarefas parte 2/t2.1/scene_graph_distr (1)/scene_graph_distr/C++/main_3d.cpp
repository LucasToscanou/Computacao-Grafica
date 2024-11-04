#include <os_specific_libs.h>

#include "arcball.h"
#include "scene.h"
#include "state.h"
#include "camera3d.h"
#include "material.h"
#include "texture.h"
#include "transform.h"
#include "cube.h"
#include "quad.h"
#include "sphere.h"
#include "error.h"
#include "shader.h"
#include "light.h"
#include "light.h"
#include "polyoffset.h"

#include <iostream>

static float viewer_pos[3] = {2.0f, 3.5f, 4.0f};

static ScenePtr scene;
static Camera3DPtr camera;
static ArcballPtr arcball;

static void initialize (void)
{
  // set background color: white 
  glClearColor(1.0f,1.0f,1.0f,1.0f);
  // enable depth test 
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);  // cull back faces

  // create objects
  camera = Camera3D::Make(viewer_pos[0],viewer_pos[1],viewer_pos[2]);
  camera->SetOrtho(true);
  arcball = camera->CreateArcball();

  // LightPtr light = ObjLight::Make(viewer_pos[0],viewer_pos[1],viewer_pos[2]);
  LightPtr light = Light::Make(2.0f, 3.0f, -2.0f, 1.0f, "camera");


  AppearancePtr white = Material::Make(1.0f,1.0f,1.0f);
  AppearancePtr red = Material::Make(1.0f,0.5f,0.5f);
  AppearancePtr yellow = Material::Make(0.5f,0.5f,0.0f);
  AppearancePtr green = Material::Make(0.0f,1.0f,0.0f);
  AppearancePtr gray = Material::Make(0.5f,0.5f,0.5f);

  // Shapes Transforms
  // Table
  TransformPtr trf_table = Transform::Make();
  trf_table->Scale(3.0f,0.3f,3.0f);
  trf_table->Translate(0.0f,-1.0f,0.0f);
  // Blcok
  TransformPtr trf_block = Transform::Make();
  trf_block->Scale(0.75f,0.25f,0.75f);
  trf_block->Translate(0.0f,0.0f,0.0f);
  // Shpere 1
  TransformPtr trf_sphere1 = Transform::Make();
  trf_sphere1->Translate(0.0f,0.35f,0.0f);
  trf_sphere1->Scale(0.1f,0.1f,0.1f);
  // Shpere 2
  TransformPtr trf_sphere2 = Transform::Make();
  trf_sphere2->Translate(0.75f,0.4f,-0.8f);
  trf_sphere2->Scale(0.4f,0.4f,0.4f);


  Error::Check("before shps");
  // Creating the shapes
  // Table
  ShapePtr table = Cube::Make();
  // Block
  ShapePtr block = Cube::Make();
  // Sphere 1
  ShapePtr sphere1 = Sphere::Make();
  // Sphere 2
  ShapePtr sphere2 = Sphere::Make();
  Error::Check("after shps");

  // create shader
  ShaderPtr shader = Shader::Make(light,"camera");
  shader->AttachVertexShader("../shaders/ilum_vert/vertex_lighting/vertex.glsl");
  shader->AttachFragmentShader("../shaders/ilum_vert/vertex_lighting/fragment.glsl");
  shader->Link();

  // Define a different shader for texture mapping
  // An alternative would be to use only this shader with a "white" texture for untextured objects
  ShaderPtr shd_tex = Shader::Make(light,"camera");
  shd_tex->AttachVertexShader("../shaders/ilum_vert/vertex_lighting/vertex_texture.glsl");
  shd_tex->AttachFragmentShader("../shaders/ilum_vert/vertex_lighting/fragment_texture.glsl");
  shd_tex->Link();


  // ShaderPtr shader = Shader::Make(light,"camera");
  // shader->AttachVertexShader("../shaders/ilum_vert/frag_lighting/vertex.glsl");
  // shader->AttachFragmentShader("../shaders/ilum_vert/frag_lighting/fragment.glsl");
  // shader->Link();

  // // Define a different shader for texture mapping
  // // An alternative would be to use only this shader with a "white" texture for untextured objects
  // ShaderPtr shd_tex = Shader::Make(light,"camera");
  // shd_tex->AttachVertexShader("../shaders/ilum_vert/frag_lighting/vertex_texture.glsl");
  // shd_tex->AttachFragmentShader("../shaders/ilum_vert/frag_lighting/fragment_texture.glsl");
  // shd_tex->Link();


  // build scene
  NodePtr root = Node::Make(shader,
    {
      Node::Make(trf_table,{gray},{table}),
      Node::Make(trf_block,{yellow},{block}),
      Node::Make(trf_sphere1,{green},{sphere1}),
      Node::Make(trf_sphere2,{red},{sphere2})
    }
  );
  scene = Scene::Make(root);
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

static void cursorpos (GLFWwindow* win, double x, double y)
{
  // convert screen pos (upside down) to framebuffer pos (e.g., retina displays)
  int wn_w, wn_h, fb_w, fb_h;
  glfwGetWindowSize(win, &wn_w, &wn_h);
  glfwGetFramebufferSize(win, &fb_w, &fb_h);
  x = x * fb_w / wn_w;
  y = (wn_h - y) * fb_h / wn_h;
  arcball->AccumulateMouseMotion(x,y);
}
static void cursorinit (GLFWwindow* win, double x, double y)
{
  // convert screen pos (upside down) to framebuffer pos (e.g., retina displays)
  int wn_w, wn_h, fb_w, fb_h;
  glfwGetWindowSize(win, &wn_w, &wn_h);
  glfwGetFramebufferSize(win, &fb_w, &fb_h);
  x = x * fb_w / wn_w;
  y = (wn_h - y) * fb_h / wn_h;
  arcball->InitMouseMotion(x,y);
  glfwSetCursorPosCallback(win, cursorpos);     // cursor position callback
}
static void mousebutton (GLFWwindow* win, int button, int action, int mods)
{
  if (action == GLFW_PRESS) {
    glfwSetCursorPosCallback(win, cursorinit);     // cursor position callback
  }
  else // GLFW_RELEASE 
    glfwSetCursorPosCallback(win, nullptr);      // callback disabled
}

int main ()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,1);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

  glfwSetErrorCallback(error);
  // GLFWwindow* win = glfwCreateWindow(800,800,"3D Scene - vertex lighting",nullptr,nullptr);
  GLFWwindow* win = glfwCreateWindow(800,800,"3D Scene - fragment lighting",nullptr,nullptr);
  if (!win) {
    std::cerr << "Could not create GLFW window" << std::endl;
    return 0;
  }
  glfwSetFramebufferSizeCallback(win, resize);  // resize callback
  glfwSetKeyCallback(win, keyboard);            // keyboard callback
  glfwSetMouseButtonCallback(win, mousebutton); // mouse button callback

  glfwMakeContextCurrent(win);
  #ifdef __glad_h_
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      printf("Failed to initialize GLAD OpenGL context\n");
      exit(1);
    }
  #endif
  printf("OpenGL version: %s\n", glGetString(GL_VERSION));


  initialize();

  while(!glfwWindowShouldClose(win)) {
    display(win);
    glfwSwapBuffers(win);
    glfwPollEvents();
  }
  glfwTerminate();
  return 0;
}

