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
  Emitter(glm::vec3 &&_pos, size_t &&_max);
  ~Emitter();

  void initTextures(std::string texPath = "textures/RadialGradient.png") const;
  void update();
  void draw() const;

  //Accessors
  inline size_t particleCount() const { return m_particleCount; }
  inline size_t maxParticles() const { return m_maxParticles;}
  inline glm::vec3 pos() const { return m_pos; }

  //Mutators
  inline void setPos(glm::vec3 &_pos) { m_pos=_pos; }
  inline void setMaxParticles(size_t &_max) { m_maxParticles = _max; }

  //Private functions
private:
  void addParticle(Particle *&&_newParticle);
  void spawnParticles();
  void createFlame();
  void createFirework();
  void createExplosion();
  void createObjects();
  void removeParticles();
  void clearParticles();

  //Public members
public:
  bool m_flame = false;
  glm::vec4 m_fiCol = glm::vec4(1.0f,0.67f,0.0f,1.0f);

  bool m_firework = false;
  glm::vec4 m_fwCol = glm::vec4(1.0f,0.078f,0.576f,1.0f);
  bool m_fwBlink = false;
  float m_fwTheta = 0.0f;
  float m_fwPhi = 0.0f;
  float m_fwThrust = 1.0f;
  int m_fwFuel = 300;
  int m_fwTrail = 20;
  int m_fwFuse = 95;
  int m_fwExpLife = 80;

  int m_explosion = 0;
  glm::vec4 m_expCol = glm::vec4(0.647f,0.306f,0.2f,1.0f);


  bool m_clear = false;
  //Private members
private:
  std::vector <std::unique_ptr<Particle>> m_particles;
  size_t m_particleCount = 0;
  size_t m_maxParticles = 50000;
  glm::vec3 m_pos = glm::vec3(0.0f,0.0f,0.0f);
  int m_frame = 0;
  int m_free = 0;
};

#endif // EMITTER_H
