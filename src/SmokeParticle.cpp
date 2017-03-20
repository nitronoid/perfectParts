#include "SmokeParticle.h"

SmokeParticle::SmokeParticle(glm::vec3 _pos,
                             glm::vec3 _vel,
                             glm::vec4 _col,
                             float _size,
                             int _life,
                             int _frame,
                             bool _spawn) : Particle( _pos,
                                                      _vel,
                                                      _col,
                                                      _size,
                                                      _life,
                                                      _frame,
                                                      _spawn)
{
  m_type = ParticleType::SMOKE;
}
SmokeParticle::~SmokeParticle()
{

}

int SmokeParticle::newParts(const int &_frame) const
{
  float initialMean = 2.0f;
  float deltaMean = -0.1f;
  float mean = round(initialMean + deltaMean * (_frame - m_birthFrame));

  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::uniform_real_distribution<float> dist(-1.0f,1.0f);
  std::mt19937_64 rng(seed);
  float rand = dist(rng);
  float variance = 0.0f;
  int num = round(mean + variance * rand);
  if ((num < 0))
  {
    num = 0;
  }
  return num;
}

glm::vec3 SmokeParticle::calcInitVel() const
{
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::uniform_real_distribution<float> dist(-1,1);
  std::mt19937_64 rng(seed);
  float variance = 0.1f;
  return glm::vec3(m_vel.x + variance * dist(rng),
                   m_vel.y + variance * dist(rng),
                   m_vel.z + variance * dist(rng));
}

//glm::vec3 SmokeParticle::getEjectionVel() const
//{
//  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//  std::uniform_real_distribution<float> dist(350,500);
//  std::mt19937_64 rng(seed);
//  float radial = dist(rng);
//  dist = std::uniform_real_distribution<float>(0.0f,360.0f);
//  float asimuthal = dist(rng);
//  dist = std::uniform_real_distribution<float>(0.0f,30.0f);
//  float polar = dist(rng);
//  glm::vec3 newVel;
//  newVel.x = (radial * cos(asimuthal) * sin(polar));
//  newVel.y = (radial * sin(asimuthal) * sin(polar));
//  newVel.z = (radial * cos(polar));
//  return newVel;
//}

Particle* SmokeParticle::createChild(const int &_frame) const
{
  return new SmokeParticle ( m_pos,                                  //initial position
                             calcInitVel(),                          //initial velocity
                             m_col,                                  //initial colour
                             m_size,                                 //initial size
                             100,                                     //life span
                             _frame,                                 //current frame
                             false);
}

void SmokeParticle::draw(const int &_frame) const
{
  glm::vec4 clampedCol = glm::clamp(m_col,0.0f,1.0f);
  glColor4fv((const GLfloat*)glm::value_ptr(clampedCol));
  glPointSize(m_size);
  //std::cout<<_p->m_pos.x<<'\t'<<_p->m_pos.y<<'\t'<<_p->m_pos.z<<'\n';
  glBegin(GL_POINTS);
  glVertex3fv((const GLfloat*)glm::value_ptr(m_pos));
  glEnd();
}
