#ifndef FIREWORKPARTICLE_H
#define FIREWORKPARTICLE_H
#include "Particle.h"

class FireworkParticle : public Particle
{
  //Public functions, constructors and destructor
public:
  FireworkParticle() = default;
  FireworkParticle( int const&_fuse,
                    glm::vec3 const&_pos,
                    glm::vec3 const&_vel,
                    glm::vec4 const&_col,
                    float const&_brightness,
                    float const&_size,
                    int const&_life,
                    int const&_elife,
                    int const&_trailLife,
                    int const&_frame,
                    bool const&_spawn,
                    bool const&_blink);

  ~FireworkParticle();

  int newParts( int const&) const override;
  void update( int const&_frame) override;
  void draw( int const&_frame) const override;
  Particle* createChild( int const&_frame) const override;

  //Private functions
private:
  void explode();
  glm::vec4 calcCol( int const&_frame) const;

  //Private members
private:
  int m_explosionFuse = 100;
  int m_blinkPeriod;
  bool m_blink = false;
  bool m_exploded = false;
  float m_brightness = 1.0f;
  int m_trailLife = 15;
  int m_explodedLife = 100;
};

#endif // FIREWORKPARTICLE_H
