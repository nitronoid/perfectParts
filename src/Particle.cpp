#include "Particle.h"

Particle::Particle(glm::vec3 _pos,
                   glm::vec3 _vel,
                   glm::vec4 _col,
                   float _size,
                   int _life,
                   int _frame,
                   bool _spawn)
{

  float sizeDecay = -(_size+1.0f)/_life;
  float colDecay = -_col.a/_life;

  m_pos = _pos;
  m_vel = _vel;
  m_accel = glm::vec3(0.0f,-0.01f,0.0f);
  m_col = _col;
  m_colDelta = glm::vec4(colDecay,colDecay,colDecay,colDecay);
  m_sizeDelta = sizeDecay;
  m_size = _size;
  m_life = _life;
  m_birthFrame = _frame;
  m_spawn = _spawn;
  m_alive = true;
}

Particle::~Particle()
{

}

void Particle::update(int const &_frame, unsigned int &_particleCount)
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
    --_particleCount;
  }
}

