#ifndef EMITTER_H
#define EMITTER_H
#include <memory>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include "Particle.h"
#include "SmokeParticle.h"
#include "FireworkParticle.h"

class Emitter
{
public:
  Emitter() = default;
  Emitter(glm::vec3 _pos, unsigned int _max);
  ~Emitter();

  void update();
  void draw() const;
  void addParticle(std::unique_ptr<Particle> &_newParticle);
  void clearParticles();
  void spawnParticles();
  void createFirework();
  void createSmoke();

  static bool compareZ(const std::unique_ptr<Particle> &_i, const std::unique_ptr<Particle> &_j);

  //Accessors
  inline unsigned int particleCount() { return m_particleCount; }
  inline glm::vec3 pos() { return m_pos; }

  //Mutators
  inline void setPos(const glm::vec3 &_pos) { m_pos=_pos;}
  bool m_smoke;

private:
  std::vector <std::unique_ptr<Particle>> m_particles;
  unsigned int m_particleCount;
  unsigned int m_maxParticles;
  glm::vec3 m_pos;
  int m_frame;
};

#endif // EMITTER_H
