#ifndef CYLINDER_H
#define CYLINDER_H

#include "shape.h"
#include "state.h"
#include <os_specific_libs.h>

class Cylinder;
using CylinderPtr = std::shared_ptr<Cylinder>;

class Cylinder : public Shape {
private:
  unsigned int m_vao, m_vbo, m_ebo;
  float m_radius, m_height;
  int m_segments;

public:
  static CylinderPtr Make(float radius = 0.5f, float height = 1.0f, int segments = 64);

  Cylinder(float radius, float height, int segments);
  virtual ~Cylinder();
  
  void Draw(StatePtr st) override;
  
  void Initialize();
};

#endif
