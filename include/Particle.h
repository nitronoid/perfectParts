#ifndef PARTICLE_H
#define PARTICLE_H

#if defined (__linux__) || defined (WIN32)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#include <glm/glm.hpp>

//------------------------------------------------------------------------------------------------------------------------
/// \file Particle.h
/// \author Jack Diver
/// \version 1.1
/// \date Last Revision 03/05/17 Updated to NCCA coding standard \n

/// Revision History: See "https://github.com/nitronoid/perfectParts"
/// Initial Version 20/03/17

/// \class Particle
/// \brief encapsulates a particle, is an abstract class
//------------------------------------------------------------------------------------------------------------------------

class Particle
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Flag telling us if the particle is currently alive
  //----------------------------------------------------------------------------------------------------------------------
  bool m_alive = true;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Flag for whether or not this particle spawns children
  //----------------------------------------------------------------------------------------------------------------------
  bool m_spawn = false;

public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Default constructor that initialises member variables to default values
  //----------------------------------------------------------------------------------------------------------------------
  Particle() = default;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor using intial position, velocity, colour, size, life span and a spawn flag
  /// @param[in] _pos the initial position of the particle
  /// @param[in] _vel the initial velocity of the particle
  /// @param[in] _col the initial colour of the particle
  /// @param[in] _size the initial size of the particle
  /// @param[in] _life the initial life of the particle
  /// @param[in] _frame the current frame, used to set m_birthFrame
  /// @param[in] _spawn used to set m_spawn, flag for spawning children
  //----------------------------------------------------------------------------------------------------------------------
  Particle( glm::vec3 const&_pos,
            glm::vec3 const&_vel,
            glm::vec4 const&_col,
            float     const&_size,
            int       const&_life,
            int       const&_frame,
            bool      const&_spawn );

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Default Virtual destructor for particle, neccessary to prevent memory leak
  //----------------------------------------------------------------------------------------------------------------------
  virtual ~Particle() = default;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Returns the amount of child particles to spawn
  /// @param[in] _frame the current frame
  //----------------------------------------------------------------------------------------------------------------------
  virtual int newParts( int const&_frame ) const = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Updates the particle properties
  /// @param[in] parameter is unused, but required to allow polymorphism across all particles
  //----------------------------------------------------------------------------------------------------------------------
  virtual void update( int const& );
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Creates a pointer to a child particle, I chose to implement the create child class as a member function
  /// for all derived particle classes as in the current system, particles may only spawn other particles of the same
  /// type. If it was required that they spawn particles of other types I would implement a factory patter, but for now
  /// this seems like a more compact solution.
  /// @param[in] _frame the current frame
  //----------------------------------------------------------------------------------------------------------------------
  virtual Particle* createChild( int const&_frame ) const = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws the particle
  /// @param[in] _frame the current frame
  //----------------------------------------------------------------------------------------------------------------------
  virtual void draw( int const&_frame ) const = 0;

protected:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The position of the particle
  //----------------------------------------------------------------------------------------------------------------------
  glm::vec3      m_pos = glm::vec3(0.0f,0.0f,0.0f);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The velocity of the particle
  //----------------------------------------------------------------------------------------------------------------------
  glm::vec3      m_vel = glm::vec3(0.0f,0.0f,0.0f);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The acceleration of the particle
  //----------------------------------------------------------------------------------------------------------------------
  glm::vec3      m_accel = glm::vec3(0.0f,-0.01f,0.0f);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The colour of the particle
  //----------------------------------------------------------------------------------------------------------------------
  glm::vec4      m_col = glm::vec4(1.0f,1.0f,1.0f,1.0f);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The rate of colour change
  //----------------------------------------------------------------------------------------------------------------------
  glm::vec4      m_colDelta = glm::vec4(0.0f,0.0f,0.0f,0.0f);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The rate of size change
  //----------------------------------------------------------------------------------------------------------------------
  GLfloat        m_sizeDelta = 0.0f;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The size of the particle
  //----------------------------------------------------------------------------------------------------------------------
  GLfloat        m_size = 5.0f;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The remaining life span in frames
  //----------------------------------------------------------------------------------------------------------------------
  int            m_life = 100;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The frame the particle was born
  //----------------------------------------------------------------------------------------------------------------------
  int            m_birthFrame = 0;

}; // end class


#endif // PARTICLE_H
