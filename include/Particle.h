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

  Particle(glm::vec3 _pos,
            glm::vec3 _vel,
            glm::vec4 _col,
            float _size,
            int _life,
            int _frame,
            bool _spawn );

  virtual ~Particle();

  virtual int newParts(const int &_frame) const = 0;
  virtual void update(const int &, size_t &_particleCount);
  virtual Particle* createChild(const int &_frame) const = 0;
  virtual void draw(const int &_frame) const = 0;
  inline ParticleType type() const { return m_type; }

  //Public members
public:
  bool m_alive;
  bool m_spawn;

  //Protected members
protected:
  glm::vec3 m_pos;
  glm::vec3 m_vel;
  glm::vec3 m_accel;
  glm::vec4 m_col;
  glm::vec4 m_colDelta;
  float m_sizeDelta;
  float m_size;
  int m_life;
  int m_birthFrame;
  ParticleType m_type;
};


#endif // PARTICLE_H
