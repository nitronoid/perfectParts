#ifndef FIREWORKPARTICLE_H
#define FIREWORKPARTICLE_H
#include "Particle.h"

class FireworkParticle : public Particle
{
  //Public functions, constructors and destructor
public:
  FireworkParticle() = default;
  FireworkParticle(const int &_fuse,
                   const glm::vec3 &_pos,
                   const glm::vec3 &_vel,
                   const glm::vec4 &_col,
                   const float &_brightness,
                   const float &_size,
                   const int &_life,
                   const int &_elife,
                   const int &_trailLife,
                   const int &_frame,
                   const bool &_spawn,
                   const bool &_blink);

  ~FireworkParticle();

  int newParts(const int &) const override;
  void update(const int &_frame) override;
  void draw(const int &_frame) const override;
  Particle* createChild(const int &_frame) const override;

  //Private functions
private:
  void explode();
  glm::vec4 calcCol(const int &_frame) const;

  //Private members
private:
  int m_explosionFuse;
  int m_blinkPeriod;
  bool m_blink;
  bool m_exploded;
  float m_brightness;
  int m_trailLife;
  int m_explodedLife;
};

#endif // FIREWORKPARTICLE_H
