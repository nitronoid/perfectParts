#ifndef FLAMEPARTICLE_H
#define FLAMEPARTICLE_H

#include "Particle.h"

//------------------------------------------------------------------------------------------------------------------------
/// \file FlameParticle.h
/// \author Jack Diver
/// \version 3.4
/// \date Last Revision 03/05/17 Updated to NCCA coding standard \n

/// Revision History: See "https://github.com/nitronoid/perfectParts"
/// Initial Version 20/03/17

/// \class FlameParticle
/// \brief encapsulates a flame particle, is derived from abstract Particle class
//------------------------------------------------------------------------------------------------------------------------

class FlameParticle : public Particle
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Default constructor that initialises member variables to default values
  //----------------------------------------------------------------------------------------------------------------------
  FlameParticle() = default;
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
  FlameParticle( glm::vec3 const&_pos,
                 glm::vec3 const&_vel,
                 glm::vec4 const&_col,
                 float     const&_size,
                 int       const&_life,
                 int       const&_frame,
                 bool      const&_spawn);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Default Virtual destructor for particle, neccessary to prevent memory leak
  //----------------------------------------------------------------------------------------------------------------------
  virtual ~FlameParticle() = default;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Returns the amount of child particles to spawn, overrides the Particle class implementation
  /// @param[in] _frame the current frame
  //----------------------------------------------------------------------------------------------------------------------
  virtual int newParts( int const&_frame ) const override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws the particle, overrides the Particle class implementation
  /// @param[in] parameter is unused, but required to allow polymorphism across all particles
  //----------------------------------------------------------------------------------------------------------------------
  virtual void draw( int const& ) const override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Creates a pointer to a child particle, overrides the Particle class implementation
  /// @param[in] _frame the current frame
  //----------------------------------------------------------------------------------------------------------------------
  virtual Particle* createChild( int const&_frame ) const override;

}; // class end

#endif // FLAMEPARTICLE_H
