#include <memory>
class Camera;
using CameraPtr = std::shared_ptr<Camera>; 

#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {
public:
  virtual glm::mat4 GetProjMatrix () const = 0;
  virtual glm::mat4 GetViewMatrix () const = 0;
};

#endif