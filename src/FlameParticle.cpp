#include "FlameParticle.h"

FlameParticle::FlameParticle(const glm::vec3 &_pos,
                             const glm::vec3 &_vel,
                             const glm::vec4 &_col,
                             const float &_size,
                             const int &_life,
                             const int &_frame,
                             const bool &_spawn) : Particle( _pos,
                                                      _vel,
                                                      _col,
                                                      _size,
                                                      _life,
                                                      _frame,
                                                      _spawn)
{
  m_type = ParticleType::FLAME;
  m_colDelta.x = 0.0f;
  m_colDelta.z = 0.3f/_life;
}
FlameParticle::~FlameParticle()
{

}

int FlameParticle::newParts(const int &_frame) const
{
  float initialMean = 2.0f;
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

Particle* FlameParticle::createChild(const int &_frame) const
{
  return new FlameParticle ( m_pos,                                  //initial position
                             m_vel + glm::ballRand(0.05f),           //initial velocity
                             m_col,                                  //initial colour
                             m_size,                                 //initial size
                             _frame-m_birthFrame+m_life,             //life span
                             _frame,                                 //current frame
                             false);
}

void FlameParticle::draw(const int &_frame) const
{
  glm::vec4 clampedCol = glm::clamp(m_col,0.0f,1.0f);
  glColor4fv((const GLfloat*)glm::value_ptr(clampedCol));
  glPointSize(m_size);
  //std::cout<<_p->m_pos.x<<'\t'<<_p->m_pos.y<<'\t'<<_p->m_pos.z<<'\n';
  glBegin(GL_POINTS);
  glVertex3fv((const GLfloat*)glm::value_ptr(m_pos));
  glEnd();
}
