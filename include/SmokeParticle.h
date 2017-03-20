#ifndef SMOKEPARTICLE_H
#define SMOKEPARTICLE_H
#include "Particle.h"

class SmokeParticle : public Particle
{
public:
  SmokeParticle(glm::vec3 _pos,
                glm::vec3 _vel,
                glm::vec4 _col,
                float _size,
                int _life,
                int _frame,
                bool _spawn);
  ~SmokeParticle();

  int newParts(const int &_frame) const override;
  glm::vec3 calcInitVel() const override;
  void draw(const int &_frame) const override;
  Particle* createChild(const int &_frame) const;

};

#endif // SMOKEPARTICLE_H
