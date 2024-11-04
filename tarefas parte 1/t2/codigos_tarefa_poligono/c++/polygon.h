#include <memory>
#include "os_specific_libs.h"

class Polygon;
using PolygonPtr = std::shared_ptr<Polygon>; 

#ifndef POLYGON_H
#define POLYGON_H

#include "shape.h"

class Polygon : public Shape {
  unsigned int m_vao;
private:
  unsigned int m_numIndices;
protected:
  Polygon (float* xy, unsigned char* rgb, unsigned int* inc, unsigned int numIndices);
public:
  static PolygonPtr Make (float* xy, unsigned char* rgb, unsigned int* inc, unsigned int numIndices);
  virtual ~Polygon ();
  virtual void Draw ();
};
#endif

