#ifndef SMOKEPARTICLE_H
#define SMOKEPARTICLE_H
#include "Particle.h"

class FlameParticle : public Particle
{
public:
  FlameParticle(glm::vec3 _pos,
                glm::vec3 _vel,
                glm::vec4 _col,
                float _size,
                int _life,
                int _frame,
                bool _spawn);
  ~FlameParticle();

  int newParts(const int &_frame) const override;
  void draw(const int &_frame) const override;
  Particle* createChild(const int &_frame) const;

};

#endif // SMOKEPARTICLE_H
