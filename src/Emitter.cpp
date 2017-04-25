#include "Emitter.h"


Emitter::Emitter(const glm::vec3 &_pos, const unsigned int &_max)
{
  m_pos = _pos;
  m_maxParticles = _max;
  m_frame = 0;
  m_particleCount = 0;
  m_free = 0;
  m_flame = false;
  m_firework = false;
  m_explosion = 0;
  m_fwCol = glm::vec4(1.0f,0.078f,0.576f,1.0f);
  m_expCol = glm::vec4(0.647f,0.306f,0.2f,1.0f);
  m_particles.reserve(_max);
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
      int index = p-m_particles.begin();
      if((!(*p)->m_alive))
      {
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
  std::unique_ptr<Particle> newParticle;
  std::vector<std::unique_ptr<Particle>>::size_type i;
  for(i = 0; i < m_particles.size(); ++i )
  {
    if(m_particles[i]->m_alive && m_particles[i]->m_spawn)
    {
      newSpawn = m_particles[i]->newParts(m_frame);
      for(int j = 0; (j < newSpawn) && (m_particleCount < m_maxParticles); ++j)
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

void Emitter::addParticle( std::unique_ptr<Particle> &_newParticle)
{
  auto deadParticle = std::find_if(m_particles.begin()+m_free,
                                   m_particles.end(),
                                   [](const std::unique_ptr<Particle> &p){return !(p->m_alive);});
  m_free = deadParticle-m_particles.begin();
  if(deadParticle != m_particles.end())
  {
    (*deadParticle) = std::move(_newParticle);
    ++m_free;
  }
  else
  {
    m_particles.emplace_back(std::move(_newParticle));
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
    glm::vec3 newVel = glm::vec3(radial * sin(phi) * cos(theta),
                                 radial * cos(phi),
                                 radial * sin(phi) * sin(theta));


    std::unique_ptr<Particle> temp (new FlameParticle(m_pos + newPos,                              //initial position
                                                      newVel,                                      //initial velocity
                                                      m_fiCol,                                     //initial colour
                                                      100.0f,                                      //initial size
                                                      40,                                          //life span
                                                      m_frame,                                     //current frame
                                                      true));                                      //flag for spawning children
    addParticle(temp);
  }
}

void Emitter::createFirework()
{
    glm::vec3 newVel = glm::vec3(m_fwThrust * sin(m_fwPhi) * cos(m_fwTheta),
                                 m_fwThrust * cos(m_fwPhi),
                                 m_fwThrust * sin(m_fwPhi) * sin(m_fwTheta));

    if(m_particleCount + m_fwFuel*(m_fwTrail*1) < m_maxParticles)
    {
      for(int i =0; i < m_fwFuel; ++i)
      {
        std::unique_ptr<Particle> temp ( new FireworkParticle(m_fwFuse,                                  //explosion timer
                                                              m_pos,                                 //initial position
                                                              newVel,                                //initial velocity
                                                              m_fwCol,                               //initial colour
                                                              1.0f,                                  //initial brightness
                                                              25.0f,                                 //initial size
                                                              m_fwFuse*2,                                //life span
                                                              m_fwExpLife,                                 //exploded life span
                                                              m_fwTrail,                                 //trail life span
                                                              m_frame,                               //current frame
                                                              true,                                  //flag for spawning trails
                                                              m_fwBlink));                           //flag for blinking
        addParticle(temp);
      }
    }

}

void Emitter::createExplosion()
{
  for(int i =0; i < 25; ++i)
  {
    float size = glm::linearRand(40.0f,120.0f);
    float theta = glm::linearRand(0.0f,6.28f);   //radians
    float phi = glm::linearRand(-1.5f,1.5f); //radians
    float radial = glm::linearRand(0.5f,1.2f);
    glm::vec3 newVel = glm::vec3(radial * sin(phi) * cos(theta),
                                 radial * cos(phi),
                                 radial * sin(phi) * sin(theta));


    std::unique_ptr<Particle> temp( new ExplosionParticle(m_pos,                                       //initial position
                                                          newVel,                                      //initial velocity
                                                          m_expCol,                                    //initial colour
                                                          size,                                        //initial size
                                                          80,                                          //life span
                                                          20,                                          //trail life spaj
                                                          m_frame,                                     //current frame
                                                          true));                                      //flag for spawning trails
    addParticle(temp);
  }
}

void Emitter::initTextures() const
{
  GLuint texID;
  QImage texImage = QImage("data/RadialGradient.png");
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

