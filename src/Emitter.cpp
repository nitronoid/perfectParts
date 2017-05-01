#include "Emitter.h"


Emitter::Emitter(glm::vec3 &&_pos, size_t &&_max) :
  m_maxParticles(std::forward<size_t>(_max)),
  m_pos(std::forward<glm::vec3>(_pos))

{
  m_particles.reserve(std::forward<size_t>(_max));
}

Emitter::~Emitter()
{
  clearParticles();
}

void Emitter::update()
{
  createObjects();
  for(auto p = m_particles.begin(); p != m_particles.end(); ++p)
  {
    if((*p)->m_alive)
    {
      (*p)->update(m_frame);
      if((!(*p)->m_alive))
      {
        int index = p-m_particles.begin();
        if(index < m_free )
        {
          m_free = index;
        }
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

void Emitter::removeParticles()
{
  for(auto it = m_particles.begin(); it != m_particles.end();)
  {
    if(!((*it)->m_alive))
    {
      it = m_particles.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

void Emitter::addParticle( Particle*  &&_newParticle)
{
  auto deadParticle = std::find_if(m_particles.begin()+m_free,
                                   m_particles.end(),
                                   [](std::unique_ptr<Particle> const&_p){return !(_p->m_alive);});
  m_free = deadParticle-m_particles.begin();
  if(deadParticle != m_particles.end())
  {
    (*deadParticle).reset(_newParticle);
    ++m_free;
  }
  else
  {
    m_particles.emplace_back(std::unique_ptr<Particle>(_newParticle));
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
  m_particles.clear();
  m_particleCount = 0;
}

void Emitter::createFlame()
{
  for(int i =0; i < 10; ++i)
  {

    glm::vec2 disk = glm::diskRand(2.0f);
    glm::vec3 newPos = glm::vec3(disk.x,0.0f,disk.y);

    float theta = glm::linearRand(0.0f,6.28f);   //radians
    float phi = glm::linearRand(-0.26f,0.26f); //radians
    float radial = 1.2f;
    glm::vec3 newVel = glm::vec3(radial * glm::sin(phi) * glm::cos(theta),
                                 radial * glm::cos(phi),
                                 radial * glm::sin(phi) * glm::sin(theta));


    addParticle( new FlameParticle(m_pos + newPos,                        //initial position
                                   newVel,                                //initial velocity
                                   m_fiCol,                               //initial colour
                                   120.0f,                                //initial size
                                   40,                                    //life span
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

void Emitter::initTextures(std::string texPath) const
{
  GLuint texID;
  QImage texImage = QImage(texPath.c_str());
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

