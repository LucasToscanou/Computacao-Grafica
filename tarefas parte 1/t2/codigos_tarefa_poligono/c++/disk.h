#include <memory>
#include "os_specific_libs.h"
#include "triangle.h"
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#ifndef DISK_H
#define DISK_H

#include "shape.h"

class Disk;
using DiskPtr = std::shared_ptr<Disk>;

class Disk : public Shape {
private:
    unsigned int m_vao;
    int m_nslice;

    float* generateDiskVertices(int nslice);

protected:
    Disk(int nslice = 64);

public:
    static DiskPtr Make(int nslice = 64);
    virtual ~Disk();
    virtual void Draw();
};

#endif // DISK_H
