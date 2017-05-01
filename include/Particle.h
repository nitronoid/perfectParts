#ifndef PARTICLE_H
#define PARTICLE_H
#if defined (__linux__) || defined (WIN32)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#include "glm/gtc/type_ptr.hpp"
#include <random>
#include <chrono>
#include <glm/glm.hpp>
#include <memory>
#include <cmath>
#include "ParticleType.h"
#include <glm/gtc/random.hpp>
#include <glm/gtx/polar_coordinates.hpp>

class Particle
{
  //Public functions, constructors and destructor
public:
  Particle() = default;
  Particle( glm::vec3 const&_pos,
            glm::vec3 const&_vel,
            glm::vec4 const&_col,
            float const&_size,
            int const&_life,
            int const&_frame,
            bool const&_spawn );

  virtual ~Particle();

  virtual int newParts( int const&_frame) const = 0;
  virtual void update( int const&);
  virtual Particle* createChild( int const&_frame) const = 0;
  virtual void draw( int const&_frame) const = 0;
  inline ParticleType type() const { return m_type; }

  //Public members
public:
  bool m_alive = true;
  bool m_spawn = false;

  //Protected members
protected:
  glm::vec3 m_pos = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 m_vel = glm::vec3(0.0f,0.0f,0.0f);
  glm::vec3 m_accel = glm::vec3(0.0f,-0.01f,0.0f);
  glm::vec4 m_col = glm::vec4(1.0f,1.0f,1.0f,1.0f);
  glm::vec4 m_colDelta;
  float m_sizeDelta;
  float m_size = 5.0f;
  int m_life = 100;
  int m_birthFrame;
  ParticleType m_type;
};


#endif // PARTICLE_H
