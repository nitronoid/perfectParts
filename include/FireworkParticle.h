#ifndef FIREWORKPARTICLE_H
#define FIREWORKPARTICLE_H
#include "Particle.h"

class FireworkParticle : public Particle
{
public:
  FireworkParticle() = default;

  FireworkParticle(int _fuse,
                   glm::vec3 _pos,
                   glm::vec3 _vel,
                   glm::vec4 _col,
                   float brightness,
                   float _size,
                   int _life,
                   int _frame,
                   bool _spawn,
                   bool _blink);

  ~FireworkParticle();

  int newParts(const int &_frame) const override;
  void update(const int &_frame, unsigned int &_particleCount) override;
  glm::vec3 calcInitVel() const override;

  void draw(const int &_frame) const override;
  Particle* createChild(const int &_frame) const override;

private:
  void explode();
  glm::vec3 getExplosionVel() const;
  glm::vec4 calcCol(const int &_frame) const;

  int m_explosionFuse;
  int m_blinkPeriod;
  bool m_blink;
  bool m_exploded;
  float m_brightness;
};

#endif // FIREWORKPARTICLE_H
