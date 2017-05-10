#include "Emitter.h"
#include "FlameParticle.h"
#include "FireworkParticle.h"
#include "ExplosionParticle.h"
#include <QImage>
#include <QtOpenGL/QGLWidget>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <iostream>

//-------------------------------------------------------------------------------------------------------------------------
/// @file Emitter.cpp
/// @brief Implementation files for Emitter class
//-------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------
Emitter::Emitter(const glm::vec3 &_pos, const size_t &_max) :
  m_maxParticles(_max),
  m_pos(_pos)

{
  //Reserve space for our particles to avoid resizing during execution
  m_particles.reserve(_max);
}
//-------------------------------------------------------------------------------------------------------------------------
Emitter::~Emitter()
{
  //clear the particle vector and free stack
  clearParticles();
}
//-------------------------------------------------------------------------------------------------------------------------
void Emitter::update()
{
  auto begin = std::chrono::high_resolution_clock::now();
  //create new particles based on user input
  createObjects();
  //update all particles
  for(auto p = m_particles.begin(); p != m_particles.end(); ++p)
  {
    //only update living particles
    if((*p)->m_alive)
    {
      (*p)->update(m_frame);

      //if particle has died from update we push it's index onto our free stack
      if((!(*p)->m_alive))
      {
        m_freeStack.push(p-m_particles.begin());
        --m_particleCount; //decrement particle count
      }
    }
  }
  //spawn new particles into the system
  spawnParticles();
  //increment frame counter
  ++m_frame;
  //end time of update
  auto end = std::chrono::high_resolution_clock::now();
  //calculate time taken vs estimated time
  auto elapsed = (end-begin).count();
  //Estimate is based on performance testing
  long estimate = 25*m_particleCount+650000;

  std::cout<<elapsed<<'\t'<<estimate<<'\n';
  //If we have no living particles, but are wasting memory on storing them, we clean up
  //If the update has taken less than average time we can spare extra time to clean up
  if(((m_particleCount <= 0) && (m_particles.size() > 0)) || (elapsed > estimate))
  {
    removeParticles(); //remove dead particles
  }
}
//-------------------------------------------------------------------------------------------------------------------------
void Emitter::removeParticles()
{
  //The following function call uses a lambda expression which ranges over the entire
  //vector of particles, and finds the ones that meet our condition, in this case that
  //the particle is dead, we then erase those particles
  m_particles.erase(std::remove_if(m_particles.begin(),
                                   m_particles.end(),
                                   [](const std::unique_ptr<Particle>& p) {return !(p->m_alive);}),
                                   m_particles.end());
  //Since we have no dead particles left we clear our stack
  std::stack<std::size_t>().swap(m_freeStack);
}
//-------------------------------------------------------------------------------------------------------------------------
void Emitter::createObjects()
{
  //based on user set variables, we clear or spawn requested particles
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
//-------------------------------------------------------------------------------------------------------------------------
void Emitter::spawnParticles()
{
  int newSpawn;
  size_t psize = m_particles.size();
  //for all particles that are both alive and allowed to spawn, we spawn new particles
  for(size_t it = 0; it < psize; ++it )
  {
    if(m_particles[it]->m_alive && m_particles[it]->m_spawn)
    {
      //obtain the amount of new particles to spawn
      newSpawn = m_particles[it]->newParts(m_frame);
      for(int spawn = 0; (spawn < newSpawn) && (m_particleCount < m_maxParticles); ++spawn)
      {
        //add the child particle
        addParticle(m_particles[it]->createChild(m_frame));
      }
    }
  }
}
//-------------------------------------------------------------------------------------------------------------------------
void Emitter::addParticle( Particle* const&_newParticle)
{
  //if our stack is empty, there are no dead particles to reset so we push back onto the vector
  if(m_freeStack.empty())
  {
    //We emplace back to avoid making a copy
    m_particles.emplace_back(std::unique_ptr<Particle>(_newParticle));
  }
  else
  {
    //if the stack isn't empty we ge the index from the top of the stack and reset the dead particle
    auto deadP = m_particles.begin() + m_freeStack.top();
    (*deadP).reset(_newParticle);
    //remove the index we just used
    m_freeStack.pop();
  }
  ++m_particleCount; //increment particle count
}
//-------------------------------------------------------------------------------------------------------------------------
void Emitter::draw() const
{
  //Save current view matrix by pushing it onto the stack
  glPushMatrix();
  //Manipulate the view with our transformation matrix
  glMultMatrixf((const GLfloat*)glm::value_ptr(m_transform));
  //Enable point sprite texturing
  glTexEnvi( GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE );
  //Draw all living particles
  for(auto const&p : m_particles)
  {
    if(p->m_alive)
    {
      p->draw(m_frame);
    }
  }
  //disable point sprite texturing so GUI draws correctly
  glTexEnvi( GL_POINT_SPRITE, GL_COORD_REPLACE, GL_FALSE );
  //Revert to our saved view
  glPopMatrix();
}
//-------------------------------------------------------------------------------------------------------------------------
void Emitter::clearParticles()
{
  //stop spawning
  m_flame = false;
  m_explosion = 0;
  //clear the vector
  m_particles.clear();
  //set particle count to zero
  m_particleCount = 0;
  //clear stack
  std::stack<std::size_t>().swap(m_freeStack);
}
//-------------------------------------------------------------------------------------------------------------------------
void Emitter::createFlame()
{
  for(int i =0; i < 10; ++i)
  {
    //random position in disk of size 2
    glm::vec2 disk = glm::diskRand(2.0f);
    glm::vec3 newPos = glm::vec3(disk.x,0.0f,disk.y);

    //life is 40 +- 20
    int life = 40;
    life = glm::linearRand(life-20,life+20);

    //get rotation, incline and speed
    float rotation = glm::linearRand(0.0f,6.28f);   //radians
    float incline = glm::linearRand(-0.26f,0.26f);   //radians
    float speed = 1.2f;

    //initial velocity from polar co-ordinates
    glm::vec3 newVel = glm::vec3(speed * glm::sin(incline) * glm::cos(rotation),
                                 speed * glm::cos(incline),
                                 speed * glm::sin(incline) * glm::sin(rotation));

    //Add the particle to our vector
    addParticle( new FlameParticle(m_pos + newPos,                        //initial position
                                   newVel,                                //initial velocity
                                   m_flCol,                               //initial colour
                                   120.0f,                                //initial size
                                   life,                                  //life span
                                   m_frame,                               //current frame
                                   true));                                //flag for spawning children
  }
}
//-------------------------------------------------------------------------------------------------------------------------
void Emitter::createFirework()
{
  //initial velocity from polar co-ordinates
  glm::vec3 newVel = glm::vec3(m_fwThrust * glm::sin(m_fwIncline) * glm::cos(m_fwRotation),
                               m_fwThrust * glm::cos(m_fwIncline),
                               m_fwThrust * glm::sin(m_fwIncline) * glm::sin(m_fwRotation));

  //check if we can fit a new particle in the system
  if(m_particleCount + m_fwFuel*(m_fwTrail*1) < m_maxParticles)
  {
    //add firework particles
    for(int i =0; i < m_fwFuel; ++i)
    {
      addParticle( new FireworkParticle(m_fwFuse,                      //explosion timer
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
//-------------------------------------------------------------------------------------------------------------------------
void Emitter::createExplosion()
{
  for(int i =0; i < 20; ++i)
  {
    //get random size, rotation, incline and speed
    float size = glm::linearRand(40.0f,100.0f);
    float rotation = glm::linearRand(0.0f,6.28f);    //radians
    float incline = glm::linearRand(-1.5f,1.5f);     //radians
    float speed = glm::linearRand(0.5f,1.2f);

    //calculate intial velocity
    glm::vec3 newVel = glm::vec3(speed * glm::sin(incline) * glm::cos(rotation),
                                 speed * glm::cos(incline),
                                 speed * glm::sin(incline) * glm::sin(rotation));

    //Add the particle to our vector
    addParticle( new ExplosionParticle(m_pos,                             //initial position
                                       newVel,                            //initial velocity
                                       m_expCol,                          //initial colour
                                       size,                              //initial size
                                       80,                                //life span
                                       20,                                //trail life span
                                       m_frame,                           //current frame
                                       true));                            //flag for spawning trails
  }
}
//-------------------------------------------------------------------------------------------------------------------------
void Emitter::initTextures(std::string const &_texPath) const
{
  GLuint texID;
  //Load image from path
  QImage texImage = QImage(_texPath.c_str());
  //convert data so that OpenGL can read it
  QImage texData = QGLWidget::convertToGLFormat(texImage);
  //generate and bind the texture
  glActiveTexture(GL_TEXTURE0);
  glGenTextures(1, &texID);
  glBindTexture(GL_TEXTURE_2D, texID);
  glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, texData.width(), texData.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texData.bits());
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  //Set textures to draw from bottom left of every point
  glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}
//-------------------------------------------------------------------------------------------------------------------------
