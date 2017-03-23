#include "FireworkParticle.h"
#include <iostream>

FireworkParticle::FireworkParticle(int _fuse,
                                   glm::vec3 _pos,
                                   glm::vec3 _vel,
                                   glm::vec4 _col,
                                   float brightness,
                                   float _size,
                                   int _life,
                                   int _trailLife,
                                   int _frame,
                                   bool _spawn,
                                   bool _blink) : Particle(_pos,
                                                           _vel,
                                                           _col,
                                                           _size,
                                                           _life,
                                                           _frame,
                                                           _spawn)
{
  m_brightness = brightness;
  m_type = ParticleType::FIREWORK;
  m_blink = _blink;

  if(_blink)
  {
    m_blinkPeriod = glm::linearRand(2,30);
  }
  else
  {
    m_blinkPeriod = 0;
  }

  m_exploded = false;
  m_explosionFuse = _fuse + _frame;
  m_trailLife = _trailLife;
}

FireworkParticle::~FireworkParticle()
{

}

int FireworkParticle::newParts(const int &_frame) const
{
  return 1;
}


void FireworkParticle::explode()
{
  m_exploded = true;
  m_vel = glm::sphericalRand(glm::linearRand(0.5f,0.75f));
  m_col += glm::vec4(glm::ballRand(0.15f),(1.0f - m_col.a));
  m_size = 2.5f;
  m_brightness = 1.0f;
  m_life = 80;

  float colChange = -m_col.a/m_life;
  m_sizeDelta = -m_size/m_life;
  m_colDelta = glm::vec4(colChange,colChange,colChange,colChange);
}

void FireworkParticle::update(int const &_frame, unsigned int &_particleCount)
{
  Particle::update(_frame,_particleCount);
  m_brightness += m_colDelta.x;
  if(!m_exploded && (m_explosionFuse == _frame)&&m_spawn)
  {
    explode();
  }
}

glm::vec4 FireworkParticle::calcCol(const int &_frame) const
{
  if((m_blink && (_frame%m_blinkPeriod <= m_blinkPeriod/2)) || !m_blink)
  {
    return glm::vec4 ((1.0f-m_col.r) * m_brightness + m_col.r,
                      (1.0f-m_col.g) * m_brightness + m_col.g,
                      (1.0f-m_col.b) * m_brightness + m_col.b,
                      m_col.a );
  }
  return glm::vec4(0.0f,0.0f,0.0f,0.0f);
}

Particle* FireworkParticle::createChild(const int &_frame) const
{
  return new FireworkParticle (-_frame,                                //we set the fuse to 0 using -frame
                               m_pos,                                  //initial position
                               glm::vec3(0.0f,0.0f,0.0f),              //initial velocity
                               m_col,                                  //initial colour
                               m_brightness,                           //initial brightness
                               m_size,                                 //initial size
                               m_trailLife,                            //life span
                               0,
                               _frame,                                 //current frame
                               false,                                  //spawning
                               m_blink);                               //blinking
}

void FireworkParticle::draw(const int &_frame) const
{
  glm::vec4 clampedCol = glm::clamp(calcCol(_frame),0.0f,1.0f);
  glColor4fv((const GLfloat*)glm::value_ptr(clampedCol));
  glPointSize(m_size);
  //std::cout<<m_pos.x<<'\t'<<m_pos.y<<'\t'<<m_pos.z<<'\n';
  glBegin(GL_POINTS);
  glVertex3fv((const GLfloat*)glm::value_ptr(m_pos));
  glEnd();
}
