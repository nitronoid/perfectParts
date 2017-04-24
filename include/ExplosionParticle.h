#ifndef EXPLOSIONPARTICLE_H
#define EXPLOSIONPARTICLE_H
#include "Particle.h"

class ExplosionParticle : public Particle
{
public:
  ExplosionParticle(const glm::vec3 &_pos,
                    const glm::vec3 &_vel,
                    const glm::vec4 &_col,
                    const float &_size,
                    const int &_life,
                    const int &_tLife,
                    const int &_frame,
                    const bool &_spawn);
  ~ExplosionParticle();

  virtual int newParts(const int &_frame) const override;
  void update(const int &_frame) override;
  virtual Particle* createChild(const int &_frame) const override;
  virtual void draw(const int &_frame) const override;
private:
  glm::vec4 calcCol() const;
public:
private:
  int m_trailLife;

};

#endif // EXPLOSIONPARTICLE_H
