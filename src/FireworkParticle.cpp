#include "FireworkParticle.h"
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>

//-------------------------------------------------------------------------------------------------------------------------
/// @file FireworkParticle.cpp
/// @brief Implementation files for Firework Particle class
//-------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------
FireworkParticle::FireworkParticle( int       const&_fuse,
                                    glm::vec3 const&_pos,
                                    glm::vec3 const&_vel,
                                    glm::vec4 const&_col,
                                    float     const&_brightness,
                                    float     const&_size,
                                    int       const&_life,
                                    int       const&_elife,
                                    int       const&_trailLife,
                                    int       const&_frame,
                                    bool      const&_spawn,
                                    bool      const&_blink) :
                                                             Particle( _pos,_vel,_col,_size,_life,_frame,_spawn),
                                                             m_trailLife(_trailLife),
                                                             //calculate frame to explode from fuse and current frame
                                                             m_explosionFuse(_fuse + _frame),
                                                             m_brightness(_brightness),
                                                             m_explodedLife(_elife),
                                                             m_blink(_blink)

{
  //if blink flag is set to true then we calculate a random period for blinking
  if(_blink)
  {
    m_blinkPeriod = glm::linearRand(2,30);
  }
  else
  {
    m_blinkPeriod = 0;
  }

}
//-------------------------------------------------------------------------------------------------------------------------
int FireworkParticle::newParts( int const&) const
{
  //spawn one particle per frame
  return 1;
}
//-------------------------------------------------------------------------------------------------------------------------
void FireworkParticle::explode()
{
  //set flag to true
  m_exploded = true;
  //calculate random velocity aligned to sphere
  m_vel = glm::sphericalRand(glm::linearRand(0.5f,0.75f));
  //Colour with random variation
  m_col += glm::vec4(glm::ballRand(0.15f),(1.0f - m_col.a));
  m_size = 25.0f;
  m_brightness = 1.0f;
  m_life = m_explodedLife;
  float colChange = -m_col.a/m_life;
  m_sizeDelta = -(m_size+1.0f)/m_life;
  m_colDelta = glm::vec4(colChange,colChange,colChange,colChange);
}
//-------------------------------------------------------------------------------------------------------------------------
void FireworkParticle::update(int const&_frame)
{
  //update as normal
  Particle::update(_frame);
  m_brightness += m_colDelta.x;
  //check whether it's time to explode
  if(!m_exploded && (m_explosionFuse == _frame)&&m_spawn)
  {
    explode();
  }
}
//-------------------------------------------------------------------------------------------------------------------------
glm::vec4 FireworkParticle::calcCol( int const&_frame) const
{
  //if the particle is blinking we need to check that it is in a visible period
  if((m_blink && (_frame%m_blinkPeriod <= m_blinkPeriod/2)) || !m_blink)
  {
    return glm::vec4 ((1.0f-m_col.r) * m_brightness + m_col.r,
                      (1.0f-m_col.g) * m_brightness + m_col.g,
                      (1.0f-m_col.b) * m_brightness + m_col.b,
                      m_col.a );
  }
  //invisible if not in blinking period
  return glm::vec4(0.0f,0.0f,0.0f,0.0f);
}
//-------------------------------------------------------------------------------------------------------------------------
Particle* FireworkParticle::createChild( int const&_frame) const
{
  //return a new firework particle
  return new FireworkParticle (-_frame,                                //we set the fuse to 0 using -frame
                               m_pos,                                  //initial position
                               glm::vec3(0.0f,0.0f,0.0f),              //initial velocity
                               m_col,                                  //initial colour
                               m_brightness,                           //initial brightness
                               m_size,                                 //initial size
                               m_trailLife,                            //life span
                               0,                                      //won't explode
                               0,                                      //won't have trails
                               _frame,                                 //current frame
                               false,                                  //spawning
                               m_blink);                               //blinking
}
//-------------------------------------------------------------------------------------------------------------------------
void FireworkParticle::draw( int const&_frame) const
{
  //clamp the colour
  glm::vec4 clampedCol = glm::clamp(calcCol(_frame),0.0f,1.0f);
  glColor4fv(glm::value_ptr(clampedCol));
  //set point size
  glPointSize(m_size);
  //draw point, must use an individual draw loop for every particle as size can't be changed from within the loop
  glBegin(GL_POINTS);
  glVertex3fv(glm::value_ptr(m_pos));
  glEnd();
}
//-------------------------------------------------------------------------------------------------------------------------
