#ifndef EXPLOSIONPARTICLE_H
#define EXPLOSIONPARTICLE_H

#include "Particle.h"

//------------------------------------------------------------------------------------------------------------------------
/// \file ExplosionParticle.h
/// \author Jack Diver
/// \version 1.1
/// \date Last Revision 03/05/17 Updated to NCCA coding standard \n

/// Revision History: \n
/// 30/04/17 Updated all functions to pass by const& \n
/// Initial Version 24/03/17

/// \class ExplosionParticle
/// \brief encapsulates an explosion particle
//------------------------------------------------------------------------------------------------------------------------

class ExplosionParticle : public Particle
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Default constructor that initialises member variables to default values
  //----------------------------------------------------------------------------------------------------------------------
  ExplosionParticle() = default;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor using intial position, velocity, colour, size, life span and a spawn flag
  /// @param[in] _pos the initial position of the particle
  /// @param[in] _vel the initial velocity of the particle
  /// @param[in] _col the initial colour of the particle
  /// @param[in] _size the initial size of the particle
  /// @param[in] _life the initial life of the particle
  /// @param[in] _tLife the life span of child particles that compose trails
  /// @param[in] _frame the current frame, used to set m_birthFrame
  /// @param[in] _spawn used to set m_spawn, flag for spawning children
  //----------------------------------------------------------------------------------------------------------------------
  ExplosionParticle( glm::vec3 const&_pos,
                     glm::vec3 const&_vel,
                     glm::vec4 const&_col,
                     float const&_size,
                     int const&_life,
                     int const&_tLife,
                     int const&_frame,
                     bool const&_spawn);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Default Virtual destructor for particle, neccessary to prevent memory leak
  //----------------------------------------------------------------------------------------------------------------------
  virtual ~ExplosionParticle() = default;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Returns the amount of child particles to spawn
  /// @param[in] parameter is unused, but required to allow polymorphism across all particles
  //----------------------------------------------------------------------------------------------------------------------
  virtual int newParts( int const&) const override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Updates the particle properties
  /// @param[in] _frame the current frame
  //----------------------------------------------------------------------------------------------------------------------
  virtual void update( int const&_frame) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Creates a pointer to a child particle
  /// @param[in] _frame the current frame
  //----------------------------------------------------------------------------------------------------------------------
  virtual Particle* createChild(int const&_frame) const override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws the particle
  /// @param[in] parameter is unused, but required to allow polymorphism across all particles
  //----------------------------------------------------------------------------------------------------------------------
  virtual void draw(int const&) const override;

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The life span of child particle trails
  //----------------------------------------------------------------------------------------------------------------------
  int            m_trailLife = 20;

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Calculates the colour of the particle for drawing
  //----------------------------------------------------------------------------------------------------------------------
  glm::vec4 calcCol() const;

}; // class end

#endif // EXPLOSIONPARTICLE_H
