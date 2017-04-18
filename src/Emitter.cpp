#include "Emitter.h"


Emitter::Emitter(const glm::vec3 &_pos, const unsigned int &_max)
{
  m_pos = _pos;
  m_maxParticles = _max;
  m_frame = 0;
  m_particleCount = 0;
  m_emitTimer = 0;
  m_flame = false;
  m_firework = true;
  m_particles.reserve(_max);
}

Emitter::~Emitter()
{
  clearParticles();
}

void Emitter::update()
{
  if(m_flame)
  {
    createFlame();
  }
  for(auto &p : m_particles)
  {
    if(p->m_alive)
    {
      p->update(m_frame,m_particleCount);
    }
  }
  spawnParticles();
  std::sort(std::begin(m_particles),std::end(m_particles),std::bind(compareZ, std::placeholders::_1, std::placeholders::_2, m_pos));
  ++m_frame;
  if(m_emitTimer > 0)
  {
    m_emitTimer--;
  }
  else
  {
    m_firework = true;
  }
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

bool Emitter::compareZ(const std::unique_ptr<Particle> &_i, const std::unique_ptr<Particle> &_j, const glm::vec3 &_origin)
{
  return (_i->zDepth(_origin)) > (_j->zDepth(_origin));
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
    m_particles.push_back(std::move(_newParticle));
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

void Emitter::createFlame()
{
  for(int i =0; i < 3; ++i)
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
                                                      glm::vec4(1.0f,0.67f,0.0f,1.0f),             //initial colour
                                                      100.0f,                                      //initial size
                                                      50,                                          //life span
                                                      m_frame,                                     //current frame
                                                      true));                                      //flag for spawning children
    addParticle(temp);
  }
}

void Emitter::createFirework()
{
  if(m_firework)
  {
    m_firework = false;
    int fuel = 100;
    int trail = 15;
    int fuse = 95;
    int eLife = 80;

    m_emitTimer = fuse + eLife + trail;

    if(m_particleCount + fuel*(trail*1) < m_maxParticles)
    {
      for(int i =0; i < fuel; ++i)
      {
        std::unique_ptr<Particle> temp ( new FireworkParticle(fuse,                                  //explosion timer
                                                              m_pos,                                 //initial position
                                                              glm::vec3(0.0f,1.0f,0.0f),             //initial velocity
                                                              glm::vec4(1.0f,0.078f,0.576f,1.0f),    //initial colour
                                                              1.0f,                                  //initial brightness
                                                              25.0f,                                 //initial size
                                                              fuse*2,                                //life span
                                                              eLife,                                 //exploded life span
                                                              trail,                                 //trail life span
                                                              m_frame,                               //current frame
                                                              true,                                  //flag for spawning trails
                                                              false));                               //flag for blinking
        addParticle(temp);
      }
    }
  }
}

void Emitter::initTextures()
{
  m_texName = "data/RadialGradient.png";
  glTexEnvi( GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE );
  QImage texImage = QImage(m_texName.c_str());
  QImage texData = QGLWidget::convertToGLFormat(texImage);
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &m_texID);
  glBindTexture(GL_TEXTURE_2D, m_texID);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, texData.width(), texData.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.bits());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}

