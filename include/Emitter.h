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
  inline glm::vec3 pos() const { return m_pos; }

  //Mutators
  inline void setPos(glm::vec3 &_pos) { m_pos=_pos;}

  //Private functions
private:
  void addParticle(std::unique_ptr<Particle> &_newParticle);
  void spawnParticles();
  void createFlame();
  void createFirework();
  void createObjects();
  void removeParticles();

  //Public members
public:
  bool m_flame;
  bool m_firework;

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
