#ifndef EXPLOSIONPARTICLE_H
#define EXPLOSIONPARTICLE_H
#include "Particle.h"

class ExplosionParticle : public Particle
{
public:
  ExplosionParticle( glm::vec3 const&_pos,
                     glm::vec3 const&_vel,
                     glm::vec4 const&_col,
                     float const&_size,
                     int const&_life,
                     int const&_tLife,
                     int const&_frame,
                     bool const&_spawn);
  ~ExplosionParticle();

  int newParts( int const&) const override;
  void update( int const&_frame) override;
  Particle* createChild(int const&_frame) const override;
  void draw(int const&) const override;
private:
  glm::vec4 calcCol() const;
public:
private:
  int m_trailLife = 20;

};

#endif // EXPLOSIONPARTICLE_H
