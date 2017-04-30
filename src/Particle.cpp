#include "Particle.h"

Particle::Particle(const glm::vec3 &_pos,
                   const glm::vec3 &_vel,
                   const glm::vec4 &_col,
                   const float &_size,
                   const int &_life,
                   const int &_frame,
                   const bool &_spawn)
{
  m_pos = _pos;
  m_vel = _vel;
  m_col = _col;
  float colDecay = -_col.a/_life;
  m_colDelta = glm::vec4(colDecay,colDecay,colDecay,colDecay);
  m_sizeDelta = -_size/_life;
  m_size = _size;
  m_life = _life;
  m_birthFrame = _frame;
  m_spawn = _spawn;
}

Particle::~Particle()
{

}

void Particle::update(int const &)
{
  if(m_alive)
  {
    m_vel += m_accel;
    m_pos += m_vel ;
    m_size += m_sizeDelta;
    m_col += m_colDelta;
    --m_life;
  }
  if(m_alive && ((m_life <= 0) || (m_size <= 1.0f) || (m_col.a <= 0.0f)))
  {
    m_alive = false;
  }
}

