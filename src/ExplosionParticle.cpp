#include "ExplosionParticle.h"

ExplosionParticle::ExplosionParticle( glm::vec3 const&_pos,
                                      glm::vec3 const&_vel,
                                      glm::vec4 const&_col,
                                      float const&_size,
                                      int const&_life,
                                      int const&_tLife,
                                      int const&_frame,
                                      bool const&_spawn) : Particle(_pos,
                                                                    _vel,
                                                                    _col,
                                                                    _size,
                                                                    _life,
                                                                    _frame,
                                                                    _spawn)
{
  m_trailLife = _tLife;
  m_type = ParticleType::EXPLOSION;
  m_sizeDelta *= 1.6f;
}

ExplosionParticle::~ExplosionParticle()
{

}

void ExplosionParticle::update(int const&_frame)
{
  if(m_spawn)
  {
    Particle::update(_frame);
  }
  else
  {
    m_pos+=m_accel;
    m_size-=m_sizeDelta;
    --m_life;
    m_col.a += m_colDelta.a;
    if(m_alive && ((m_life <= 0) || (m_col.a <= 0.0f)))
    {
      m_alive = false;
    }
  }
}

int ExplosionParticle::newParts(int const&) const
{
  return 1;
}

Particle* ExplosionParticle::createChild( int const&_frame) const
{
  return new ExplosionParticle ( m_pos + glm::ballRand(0.05f),           //initial position
                                 glm::ballRand(0.01f),                   //initial velocity
                                 m_col,                                  //initial colour
                                 m_size,                                 //initial size
                                 m_life,                                 //life span
                                 0,                                      //trail life
                                 _frame,                                 //current frame
                                 false);                                 //spawn
}

glm::vec4 ExplosionParticle::calcCol() const
{
  return glm::vec4 (m_col.r * m_col.a,
                    m_col.g * m_col.a,
                    m_col.b * m_col.a,
                    m_col.a );
}

void ExplosionParticle::draw( int const&) const
{
  glm::vec4 clampedCol = glm::clamp(calcCol(),0.0f,1.0f);
  glColor4fv((const GLfloat*)glm::value_ptr(clampedCol));
  glPointSize(m_size);
  //std::cout<<_p->m_pos.x<<'\t'<<_p->m_pos.y<<'\t'<<_p->m_pos.z<<'\n';
  glBegin(GL_POINTS);
  glVertex3fv((const GLfloat*)glm::value_ptr(m_pos));
  glEnd();
}

