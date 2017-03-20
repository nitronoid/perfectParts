#include "Emitter.h"

Emitter::Emitter(glm::vec3 _pos, unsigned int _max)
{
  m_pos = _pos;
  m_maxParticles = _max;
  m_frame = 0;
  m_particleCount = 0;
  m_smoke = false;
}

Emitter::~Emitter()
{
  clearParticles();
}

void Emitter::update()
{
  if(m_smoke)
  {
    createSmoke();
  }
  spawnParticles();
  for(auto &p : m_particles)
  {
    if(p->m_alive)
    {
      p->update(m_frame,m_particleCount);
    }
  }

  std::sort(std::begin(m_particles),std::end(m_particles),compareZ);
  ++m_frame;
}

void Emitter::spawnParticles()
{
  int newSpawn;
  std::unique_ptr<Particle> newParticle;
  for(std::vector<std::unique_ptr<Particle>>::size_type i = 0; i < m_particles.size(); ++i )
  {
    if(m_particles[i]->m_alive && m_particles[i]->m_spawn)
    {
      newSpawn = m_particles[i]->newParts(m_frame);
      for(int i = 0; (i < newSpawn) && (m_particleCount < m_maxParticles); ++i)
      {
        newParticle.reset(m_particles[i]->createChild(m_frame));
        if(newParticle != nullptr)
        {
          addParticle(newParticle);
        }
      }
    }
  }
}
bool Emitter::compareZ(const std::unique_ptr<Particle> &_i, const std::unique_ptr<Particle> &_j)
{
  return (_i->alphaBlend()) < (_j->alphaBlend());
}


void Emitter::addParticle( std::unique_ptr<Particle> &_newParticle)
{
  bool isSpace = false;
  for(auto &i : m_particles)
  {
    if(!i->m_alive)
    {
      i = std::move(_newParticle);
      ++m_particleCount;
      isSpace = true;
      break;
    }
  }
  if(!isSpace && (m_particleCount < m_maxParticles))
  {
    m_particles.emplace_back(std::move(_newParticle));
    ++m_particleCount;
  }

}

void Emitter::draw() const
{
  for(auto &p : m_particles)
  {
    if(p->m_alive)
    {
      p->draw(m_frame);
    }
  }
}

void Emitter::clearParticles()
{
  m_particles.clear();
  m_particleCount = 0;
}

void Emitter::createSmoke()
{
  std::unique_ptr<Particle> temp (new SmokeParticle(m_pos,                                  //initial position
                                                    glm::vec3(-0.25f,1.0f,0.0f),  //initial velocity
                                                    glm::vec4(1.0f,1.0f,1.0f,1.0f),                              //initial colour
                                                    2.0f,                                 //initial size
                                                    100,                            //life span
                                                    m_frame,                                        //current frame
                                                    true));
  addParticle(temp);
}

void Emitter::createFirework()
{
  int fuel = 200;
  int trail = 10;
  if(m_particleCount + fuel*(trail*1) < m_maxParticles)
  {
    for(int i =0; i < fuel; ++i)
    {
      std::unique_ptr<Particle> temp ( new FireworkParticle(95,
                                                            m_pos,                                  //initial position
                                                            glm::vec3(0.0f,1.0f,0.0f),  //initial velocity
                                                            glm::vec4(1.0f,0.078f,0.576f,1.0f),                              //initial colour
                                                            1.0f,                           //initial brightness
                                                            2.5f,                                 //initial size
                                                            200,                            //life span
                                                            m_frame,                                        //current frame
                                                            true,
                                                            false));
      addParticle(temp);
    }
  }
}
