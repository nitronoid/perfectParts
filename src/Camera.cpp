#include "Camera.h"
#include <cmath>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

Camera::Camera()
{
  m_pos = glm::vec3(0.0f, 0.0f, 3.0f);
  m_target = glm::vec3(0.0f, 0.0f, 0.0f);
  m_direction = glm::normalize(m_pos - m_target);
  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
  m_right = glm::normalize(glm::cross(up, m_direction));
  m_up = glm::cross(m_direction, m_right);
  m_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f),
                       glm::vec3(0.0f, 0.0f, 0.0f),
                       glm::vec3(0.0f, 1.0f, 0.0f));

}

glm::mat4 Camera::genTranslateMat(const float _x, const float _y, const float _z)
{
  glm::mat4 transformation( 1,  0,  0, _x,
                            0,  1,  0, _y,
                            0,  0,  1, _z,
                            0,  0,  0,  1 );
  return transformation;
}

glm::mat4 Camera::genRotateMat(const float _x, const float _y, const float _z)
{
  glm::mat4 rotateX( 1,        0,        0,  0,
                     0,  cos(_x), -sin(_x),  0,
                     0,  sin(_x),  cos(_x),  0,
                     0,        0,        0,  1 );

  glm::mat4 rotateY( cos(_y),  0,  sin(_y),  0,
                           0,  1,        0,  0,
                    -sin(_y),  0,  cos(_y),  0,
                           0,  0,        0,  1 );

  glm::mat4 rotateZ( cos(_z), -sin(_z), 0,  0,
                     sin(_z),  cos(_z), 0,  0,
                           0,        0, 1,  0,
                           0,        0, 0,  1 );
  glm::mat4 transformation = rotateX * rotateY * rotateZ;

  return transformation;
}

void Camera::move(const float _x, const float _y, const float _z, const bool _key)
{
  glm::mat4 transformation;
  if (_key)
  {
    transformation = genTranslateMat(_x,_y,_z);
  }
  else
  {
    transformation = genRotateMat(_x,_y,_z);
  }
  move(transformation);
}

void Camera::move(glm::mat4 _transformation)
{
  glm::vec4 pos4(m_pos,1.0);
  pos4 = _transformation * pos4;
  m_pos = glm::vec3(pos4);
}

