#include <memory>
class Disk;
using DiskPtr = std::shared_ptr<Disk>; 

#ifndef DISK_H
#define DISK_H

#include "shape.h"

class Disk : public Shape {
  unsigned int m_vao;
  int m_nslice;
  float m_radius;
private:
  float* generateDiskVertices();
  float* generateDiskTexCoord();
protected:
  Disk (int nslice, float radius);
public:
  static DiskPtr Make (int nslice= 64, float radius=1.0f);
  virtual ~Disk ();
  virtual void Draw (StatePtr st);
};
#endif