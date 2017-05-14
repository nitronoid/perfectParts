#include "FlameParticle.h"
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <array>

//-------------------------------------------------------------------------------------------------------------------------
/// @file FireworkParticle.cpp
/// @brief Implementation files for Firework Particle class
//-------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------
FlameParticle::FlameParticle( glm::vec3 const&_pos,
                              glm::vec3 const&_vel,
                              glm::vec4 const&_col,
                              float const&_size,
                              int const&_life,
                              int const&_frame,
                              bool const&_spawn) :
                                                  Particle(_pos,_vel,_col,_size,_life,_frame,_spawn)


{
  //double size delta to shorten frame
  m_sizeDelta *= 2.0f;
  //don't decay red and only a small amount of blue, this simulates burning colours
  m_colDelta.r = 0.0f;
  m_colDelta.b = 0.15f/_life;
}
//-------------------------------------------------------------------------------------------------------------------------
int FlameParticle::newParts( int const&_frame) const
{
  //gradual decay in particle spawning based on the WILLIAM T.REEVES [1983] method
  float initialMean = 1.75f;
  float deltaMean = -0.1f;
  float mean = initialMean + deltaMean * (_frame - m_birthFrame);
  float variance = 0.2f;
  int num = round(mean + variance * glm::linearRand(-1.0f,1.0f));
  if ((num < 0))
  {
    num = 0;
  }
  return num;
}
//-------------------------------------------------------------------------------------------------------------------------
Particle* FlameParticle::createChild( int const&_frame ) const
{
  //return new flame particle
  return new FlameParticle ( m_pos,                                  //initial position
                             m_vel + glm::ballRand(0.05f),           //initial velocity
                             m_col,                                  //initial colour
                             m_size,                                 //initial size
                             _frame-m_birthFrame+m_life,             //life span
                             _frame,                                 //current frame
                             false);
}
//-------------------------------------------------------------------------------------------------------------------------
void FlameParticle::draw( int const& ) const
{
  //clamp the colour
  glm::vec4 clampedCol = glm::clamp(m_col,0.0f,1.0f);
  glColor4fv(glm::value_ptr(clampedCol));
  //set point size
  glPointSize(m_size);
  //draw point, must use an individual draw loop for every particle as size can't be changed from within the loop
  glBegin(GL_POINTS);
  glVertex3fv(glm::value_ptr(m_pos));
  glEnd();
}
//-------------------------------------------------------------------------------------------------------------------------
