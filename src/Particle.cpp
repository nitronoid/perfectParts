#include "Particle.h"

//-------------------------------------------------------------------------------------------------------------------------
/// @file Particle.cpp
/// @brief Implementation files for Particle class
//-------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------
Particle::Particle( glm::vec3 const&_pos,
                    glm::vec3 const&_vel,
                    glm::vec4 const&_col,
                    float const&_size,
                    int const&_life,
                    int const&_frame,
                    bool const&_spawn)
{
  m_pos = _pos;
  m_vel = _vel;
  m_col = _col;
  //colour and size decay is based on life span
  GLfloat colDecay = -_col.a/_life;
  m_colDelta = glm::vec4(colDecay,colDecay,colDecay,colDecay);
  m_sizeDelta = -_size/_life;
  m_size = _size;
  m_life = _life;
  m_birthFrame = _frame;
  m_spawn = _spawn;
}
//-------------------------------------------------------------------------------------------------------------------------
void Particle::update(int const&)
{
  //update if alive
  if(m_alive)
  {
    m_vel += m_accel;
    m_pos += m_vel ;
    m_size += m_sizeDelta;
    m_col += m_colDelta;
    --m_life;
  }
  //if life is zero, colour is invisible, or size smaller than a pixel, set to dead
  if(m_alive && ((m_life <= 0) || (m_size <= 1.0f) || (m_col.a <= 0.0f)))
  {
    m_alive = false;
  }
}
//-------------------------------------------------------------------------------------------------------------------------
