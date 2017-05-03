#ifndef FLAMEPARTICLE_H
#define FLAMEPARTICLE_H
#include "Particle.h"

class FlameParticle : public Particle
{
  //Public functions, constructors and destructor
public:
  FlameParticle() = default;
  FlameParticle( glm::vec3 const&_pos,
                 glm::vec3 const&_vel,
                 glm::vec4 const&_col,
                 float const&_size,
                 int const&_life,
                 int const&_frame,
                 bool const&_spawn);
  ~FlameParticle();

  int newParts( int const&_frame) const override;
  void draw( int const&) const override;
  Particle* createChild( int const&_frame) const override;
};

#endif // FLAMEPARTICLE_H
