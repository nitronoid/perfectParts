#include "Emitter.h"
//#include <omp.h>

Emitter::Emitter(const glm::vec3 &_pos, const size_t &_max) :
  m_maxParticles(_max),
  m_pos(_pos)

{
  m_particles.reserve(_max);
}

Emitter::~Emitter()
{
  clearParticles();
}

void Emitter::update()
{
  createObjects();
//  #pragma omp parallel for
//  for(size_t i = 0; i < m_particles.size(); ++i)
//  {
//    if((m_particles[i])->m_alive)
//    {
//      (m_particles[i])->update(m_frame);
//      #pragma omp critical
//      if((!(m_particles[i])->m_alive))
//      {
//        --m_particleCount;
//        m_freeStack.push(i);

//      }
//    }
//  }
  for(auto p = m_particles.begin(); p != m_particles.end(); ++p)
  {
    if((*p)->m_alive)
    {
      (*p)->update(m_frame);
      if((!(*p)->m_alive))
      {
        m_freeStack.push(p-m_particles.begin());
        --m_particleCount;
      }
    }
  }
  spawnParticles();
  ++m_frame;
}

void Emitter::createObjects()
{
  if(m_clear)
  {
    clearParticles();
    m_clear = false;
  }
  if(m_flame)
  {
    createFlame();
  }
  if(m_firework)
  {
    createFirework();
    m_firework = false;
  }
  if(m_explosion > 0)
  {
    createExplosion();
    m_explosion--;
  }
}

void Emitter::spawnParticles()
{
  int newSpawn;
  std::vector<std::unique_ptr<Particle>>::size_type it;
  for(it = 0; it < m_particles.size(); ++it )
  {
    if(m_particles[it]->m_alive && m_particles[it]->m_spawn)
    {
      newSpawn = m_particles[it]->newParts(m_frame);
      for(int spawn = 0; (spawn < newSpawn) && (m_particleCount < m_maxParticles); ++spawn)
      {
        addParticle(m_particles[it]->createChild(m_frame));
      }
    }
  }
}

void Emitter::addParticle( Particle*  &&_newParticle)
{
  if(m_freeStack.empty())
  {
    m_particles.emplace_back(std::unique_ptr<Particle>(_newParticle));
  }
  else
  {
    auto deadP = m_particles.begin() + m_freeStack.top();
    (*deadP).reset(_newParticle);
    m_freeStack.pop();
  }
  ++m_particleCount;
}

void Emitter::draw() const
{
  glTexEnvi( GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE );
  for(auto &p : m_particles)
  {
    if(p->m_alive)
    {
      p->draw(m_frame);
    }
  }
  glTexEnvi( GL_POINT_SPRITE, GL_COORD_REPLACE, GL_FALSE );
}

void Emitter::clearParticles()
{
  m_flame = false;
  m_explosion = 0;
  m_particles.clear();
  m_particleCount = 0;
  std::stack<std::size_t>().swap(m_freeStack);
}

void Emitter::createFlame()
{
  for(int i =0; i < 10; ++i)
  {

    glm::vec2 disk = glm::diskRand(2.0f);
    glm::vec3 newPos = glm::vec3(disk.x,0.0f,disk.y);


    int life = 40;
    life = glm::linearRand(life-20,life+20);

    float theta = glm::linearRand(0.0f,6.28f);   //radians
    float phi = glm::linearRand(-0.26f,0.26f); //radians
    float radial = 1.2f;
    glm::vec3 newVel = glm::vec3(radial * glm::sin(phi) * glm::cos(theta),
                                 radial * glm::cos(phi),
                                 radial * glm::sin(phi) * glm::sin(theta));


    addParticle( new FlameParticle(m_pos + newPos,                        //initial position
                                   newVel,                                //initial velocity
                                   m_flCol,                               //initial colour
                                   120.0f,                                //initial size
                                   life,                                    //life span
                                   m_frame,                               //current frame
                                   true));                                //flag for spawning children
  }
}

void Emitter::createFirework()
{
  glm::vec3 newVel = glm::vec3(m_fwThrust * glm::sin(m_fwPhi) * glm::cos(m_fwTheta),
                               m_fwThrust * glm::cos(m_fwPhi),
                               m_fwThrust * glm::sin(m_fwPhi) * glm::sin(m_fwTheta));

  if(m_particleCount + m_fwFuel*(m_fwTrail*1) < m_maxParticles)
  {
    for(int i =0; i < m_fwFuel; ++i)
    {
      addParticle( new FireworkParticle(m_fwFuse,                         //explosion timer
                                        m_pos,                         //initial position
                                        newVel,                        //initial velocity
                                        m_fwCol,                       //initial colour
                                        1.0f,                          //initial brightness
                                        25.0f,                         //initial size
                                        m_fwFuse*2,                    //life span
                                        m_fwExpLife,                   //exploded life span
                                        m_fwTrail,                     //trail life span
                                        m_frame,                       //current frame
                                        true,                          //flag for spawning trails
                                        m_fwBlink));                   //flag for blinking

    }
  }

}

void Emitter::createExplosion()
{
  for(int i =0; i < 20; ++i)
  {
    float size = glm::linearRand(40.0f,100.0f);
    float theta = glm::linearRand(0.0f,6.28f);   //radians
    float phi = glm::linearRand(-1.5f,1.5f); //radians
    float radial = glm::linearRand(0.5f,1.2f);
    glm::vec3 newVel = glm::vec3(radial * glm::sin(phi) * glm::cos(theta),
                                 radial * glm::cos(phi),
                                 radial * glm::sin(phi) * glm::sin(theta));


    addParticle( new ExplosionParticle(m_pos,                             //initial position
                                       newVel,                            //initial velocity
                                       m_expCol,                          //initial colour
                                       size,                              //initial size
                                       80,                                //life span
                                       20,                                //trail life span
                                       m_frame,                           //current frame
                                       true));                           //flag for spawning trails
  }
}

void Emitter::initTextures(std::string const &_texPath) const
{
  GLuint texID;
  QImage texImage = QImage(_texPath.c_str());
  QImage texData = QGLWidget::convertToGLFormat(texImage);
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, texData.width(), texData.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.bits());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}

