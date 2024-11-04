#include "cylinder.h"
#include <cmath>
#include <vector>

CylinderPtr Cylinder::Make(float radius, float height, int segments) {
  return CylinderPtr(new Cylinder(radius, height, segments));
}

Cylinder::Cylinder(float radius, float height, int segments)
  : m_radius(radius), m_height(height), m_segments(segments) {
  Initialize();
}

Cylinder::~Cylinder() {
  glDeleteVertexArrays(1, &m_vao);
  glDeleteBuffers(1, &m_vbo);
  glDeleteBuffers(1, &m_ebo);
}

void Cylinder::Initialize() {
  float* coord = new float[3*(nstack+1)*(nslice+1)];
  float* tangent = new float[3*(nstack+1)*(nslice+1)];
  float* texcoord = new float[2*(nstack+1)*(nslice+1)];  
}

void Cylinder::Draw(StatePtr st) {
  
}
