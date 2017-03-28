#ifndef EMITTER_H
#define EMITTER_H
#include <memory>
#include <vector>
#include <algorithm>
#include <glm/glm.hpp>
#include <QObject>
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

  void update();
  void draw() const;
  void addParticle(std::unique_ptr<Particle> &_newParticle);
  void clearParticles();
  void spawnParticles();
  void createFirework();
  void createFlame();
  void initTextures();

  //Accessors
  inline unsigned int particleCount() const { return m_particleCount; }
  inline glm::vec3 pos() const { return m_pos; }

  //Mutators
  inline void setPos( glm::vec3 &_pos) { m_pos=_pos;}

  //Private functions
private:
  static bool compareZ(const std::unique_ptr<Particle> &_i, const std::unique_ptr<Particle> &_j, const float &_origin);

  //Public members
public:
  bool m_smoke;

  //Private members
private:
  std::vector <std::unique_ptr<Particle>> m_particles;
  unsigned int m_particleCount;
  unsigned int m_maxParticles;
  glm::vec3 m_pos;
  int m_frame;
  GLuint m_texID;
  std::string m_texName;

};

#endif // EMITTER_H
