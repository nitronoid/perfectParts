#ifndef SMOKEPARTICLE_H
#define SMOKEPARTICLE_H
#include "Particle.h"

class FlameParticle : public Particle
{
  //Public functions, constructors and destructor
public:
  FlameParticle(const glm::vec3 &_pos,
                const glm::vec3 &_vel,
                const glm::vec4 &_col,
                const float &_size,
                const int &_life,
                const int &_frame,
                const bool &_spawn);
  ~FlameParticle();

  int newParts(const int &_frame) const override;
  void draw(const int &) const override;
  Particle* createChild(const int &_frame) const override;
};

#endif // SMOKEPARTICLE_H
