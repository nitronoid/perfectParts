#include "ExplosionParticle.h"
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>

//-------------------------------------------------------------------------------------------------------------------------
/// @file ExplosionParticle.cpp
/// @brief Implementation files for Explosion Particle class
//-------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------
ExplosionParticle::ExplosionParticle( glm::vec3 const&_pos,
                                      glm::vec3 const&_vel,
                                      glm::vec4 const&_col,
                                      float const&_size,
                                      int const&_life,
                                      int const&_tLife,
                                      int const&_frame,
                                      bool const&_spawn) :
                                                           Particle(_pos,
                                                                    _vel,
                                                                    _col,
                                                                    _size,
                                                                    _life,
                                                                    _frame,
                                                                    _spawn)
{
  m_trailLife = _tLife;
  //increase the rate of size change
  m_sizeDelta *= 1.5f;
}
//-------------------------------------------------------------------------------------------------------------------------
void ExplosionParticle::update(int const&_frame)
{
  //if it's a spawning particle we update as normal
  if(m_spawn)
  {
    Particle::update(_frame);
  }
  else
  { //if it's a trail we increase size instead of decay, and we don't use size to check if dead
    m_pos+=m_accel;
    m_size-=m_sizeDelta;
    --m_life;
    m_col.a += m_colDelta.a;
    //if life has run out, or particle isn't visible we set flag to dead
    if(m_alive && ((m_life <= 0) || (m_col.a <= 0.0f)))
    {
      m_alive = false;
    }
  }
}
//-------------------------------------------------------------------------------------------------------------------------
int ExplosionParticle::newParts(int const&) const
{
  //spawn one trail particle per frame
  return 1;
}

Particle* ExplosionParticle::createChild( int const&_frame) const
{
  //return a new explosion particle as child
  return new ExplosionParticle ( m_pos + glm::ballRand(0.05f),   //Child particle takes current position with degree of randomness
                                 glm::ballRand(0.01f),           //Intial velocity of child is small random value
                                 m_col,                          //initial colour inherited
                                 m_size,                         //initial size inherited
                                 m_life,                         //life span inherited
                                 0,                              //trail life is zero as this won't spawn children
                                 _frame,                         //current frame
                                 false);                         //Set to false so we don't spawn from the child
}
//-------------------------------------------------------------------------------------------------------------------------
glm::vec4 ExplosionParticle::calcCol() const
{
  //multiply rgb by alpha value
  return glm::vec4 (m_col.r * m_col.a,
                    m_col.g * m_col.a,
                    m_col.b * m_col.a,
                    m_col.a );
}
//-------------------------------------------------------------------------------------------------------------------------
void ExplosionParticle::draw( int const&) const
{
  //clamp the colour
  glm::vec4 clampedCol = glm::clamp(calcCol(),0.0f,1.0f);
  glColor4fv((const GLfloat*)glm::value_ptr(clampedCol));
  //set point size
  glPointSize(m_size);
  //draw point, must use an individual draw loop for every particle as size can't be changed from within the loop
  glBegin(GL_POINTS);
  glVertex3fv((const GLfloat*)glm::value_ptr(m_pos));
  glEnd();
}
//-------------------------------------------------------------------------------------------------------------------------
