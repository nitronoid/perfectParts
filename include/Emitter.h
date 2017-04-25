#ifndef EMITTER_H
#define EMITTER_H
#include <memory>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <QImage>
#include <QtOpenGL/QGLWidget>
#include "pngutils.h"
#include "Particle.h"
#include "FlameParticle.h"
#include "FireworkParticle.h"
#include "ExplosionParticle.h"

class Emitter
{
  //Public functions, contructors and destructor
public:
  Emitter() = default;
  Emitter(const glm::vec3 &_pos, const unsigned int &_max);
  ~Emitter();

  void initTextures() const;
  void update();
  void draw() const;
  void clearParticles();

  //Accessors
  inline size_t particleCount() const { return m_particleCount; }
  inline size_t maxParticles() const { return m_maxParticles;}
  inline glm::vec3 pos() const { return m_pos; }

  //Mutators
  inline void setPos(glm::vec3 &_pos) { m_pos=_pos; }
  inline void setMaxParticles(size_t &_max) { m_maxParticles = _max; }

  //Private functions
private:
  void addParticle(std::unique_ptr<Particle> &_newParticle);
  void spawnParticles();
  void createFlame();
  void createFirework();
  void createExplosion();
  void createObjects();
  void removeParticles();

  //Public members
public:
  bool m_flame;
  glm::vec4 m_fiCol = glm::vec4(1.0f,0.67f,0.0f,1.0f);

  bool m_firework;
  glm::vec4 m_fwCol;
  bool m_fwBlink = false;
  float m_fwTheta = 3.14f;
  float m_fwPhi = 0.0f;
  float m_fwThrust = 1.0f;
  int m_fwFuel = 300;
  int m_fwTrail = 20;
  int m_fwFuse = 95;
  int m_fwExpLife = 80;

  int m_explosion;
  glm::vec4 m_expCol;

  //Private members
private:
  std::vector <std::unique_ptr<Particle>> m_particles;
  size_t m_particleCount;
  size_t m_maxParticles;
  glm::vec3 m_pos;
  int m_frame;
  int m_free;
};

#endif // EMITTER_H
