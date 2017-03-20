#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>

class Camera
{
public:
  Camera();
  void move(const float _x, const float _y, const float _z, const bool _key);
  void move(glm::mat4 _transformation);
  void loadProjection();

private:

  glm::mat4 genTranslateMat(const float _x, const float _y, const float _z);
  glm::mat4 genRotateMat(const float _x, const float _y, const float _z);

  glm::vec3 m_pos;
  glm::vec3 m_direction;
  glm::vec3 m_target;
  glm::vec3 m_right;
  glm::vec3 m_up;
  glm::mat4 m_view;
};

#endif // CAMERA_H
