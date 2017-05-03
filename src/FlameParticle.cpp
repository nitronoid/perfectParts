#include "FlameParticle.h"

FlameParticle::FlameParticle( glm::vec3 const&_pos,
                              glm::vec3 const&_vel,
                              glm::vec4 const&_col,
                              float const&_size,
                              int const&_life,
                              int const&_frame,
                              bool const&_spawn) : Particle(_pos,
                                                            _vel,
                                                            _col,
                                                            _size,
                                                            _life,
                                                            _frame,
                                                            _spawn)
{
  m_sizeDelta *= 2.0f;
  m_colDelta.r = 0.0f;
  m_colDelta.b = 0.15f/_life;
}
FlameParticle::~FlameParticle()
{

}

int FlameParticle::newParts( int const&_frame) const
{
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

Particle* FlameParticle::createChild( int const&_frame) const
{
  return new FlameParticle ( m_pos,                                  //initial position
                             m_vel + glm::ballRand(0.05f),           //initial velocity
                             m_col,                                  //initial colour
                             m_size,                                 //initial size
                             _frame-m_birthFrame+m_life,             //life span
                             _frame,                                 //current frame
                             false);
}

void FlameParticle::draw( int const&) const
{
  glm::vec4 clampedCol = glm::clamp(m_col,0.0f,1.0f);
  glColor4fv((const GLfloat*)glm::value_ptr(clampedCol));
  glPointSize(m_size);
  //std::cout<<_p->m_pos.x<<'\t'<<_p->m_pos.y<<'\t'<<_p->m_pos.z<<'\n';
  glBegin(GL_POINTS);
  glVertex3fv((const GLfloat*)glm::value_ptr(m_pos));
  glEnd();
}
