#ifndef FIREWORKPARTICLE_H
#define FIREWORKPARTICLE_H

#include "Particle.h"

//------------------------------------------------------------------------------------------------------------------------
/// \file FireworkParticle.h
/// \author Jack Diver
/// \version 2.1
/// \date Last Revision 03/05/17 Updated to NCCA coding standard \n

/// Revision History: See "https://github.com/nitronoid/perfectParts"
/// Initial Version 20/03/17

/// \class FireworkParticle
/// \brief encapsulates a firework particle, is derived from abstract Particle class
//------------------------------------------------------------------------------------------------------------------------

class FireworkParticle : public Particle
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Default constructor that initialises member variables to default values
  //----------------------------------------------------------------------------------------------------------------------
  FireworkParticle() = default;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor using intial position, velocity, colour, size, life span and a spawn flag
  /// @param[in] _fuse the amount of frames until the firework explodes
  /// @param[in] _pos the initial position of the particle
  /// @param[in] _vel the initial velocity of the particle
  /// @param[in] _col the initial colour of the particle
  /// @param[in] _brightness the intial brightness of the particle
  /// @param[in] _size the initial size of the particle
  /// @param[in] _life the initial life of the particle
  /// @param[in] _eLife the life of the particle after exploding
  /// @param[in] _trailLife the life span of child particles that compose trails
  /// @param[in] _frame the current frame, used to set m_birthFrame
  /// @param[in] _spawn used to set m_spawn, flag for spawning children
  /// @param[in] _blink used to set m_blink, flag for blinking, or sparkling effect
  //----------------------------------------------------------------------------------------------------------------------
  FireworkParticle( int       const&_fuse,
                    glm::vec3 const&_pos,
                    glm::vec3 const&_vel,
                    glm::vec4 const&_col,
                    float     const&_brightness,
                    float     const&_size,
                    int       const&_life,
                    int       const&_elife,
                    int       const&_trailLife,
                    int       const&_frame,
                    bool      const&_spawn,
                    bool      const&_blink );

  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Default Virtual destructor for particle, neccessary to prevent memory leak
  //----------------------------------------------------------------------------------------------------------------------
  virtual ~FireworkParticle() = default;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Returns the amount of child particles to spawn, overrides the Particle class implementation
  /// @param[in] parameter is unused, but required to allow polymorphism across all particles
  //----------------------------------------------------------------------------------------------------------------------
  virtual int newParts( int const& ) const override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Updates the particle properties, overrides the Particle class implementation
  /// @param[in] _frame the current frame
  //----------------------------------------------------------------------------------------------------------------------
  virtual void update( int const&_frame ) override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws the particle, overrides the Particle class implementation
  /// @param[in] _frame the current frame
  //----------------------------------------------------------------------------------------------------------------------
  virtual void draw( int const&_frame ) const override;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Creates a pointer to a child particle, overrides the Particle class implementation
  /// @param[in] _frame the current frame
  //----------------------------------------------------------------------------------------------------------------------
  virtual Particle* createChild( int const&_frame ) const override;

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The life span of child particle trails
  //----------------------------------------------------------------------------------------------------------------------
  int            m_trailLife = 20;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The fuse timer in frames until the particle explodes
  //----------------------------------------------------------------------------------------------------------------------
  int            m_explosionFuse = 100;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The time in frames that the particle alternates between being visible and not visible
  //----------------------------------------------------------------------------------------------------------------------
  int            m_blinkPeriod = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The brightness of the particle used to calculate draw colour, simulates temperature of firework
  //----------------------------------------------------------------------------------------------------------------------
  float          m_brightness = 1.0f;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The life of the particle once it has exploded
  //----------------------------------------------------------------------------------------------------------------------
  int            m_explodedLife = 100;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Flag for blinking particle
  //----------------------------------------------------------------------------------------------------------------------
  bool           m_blink = false;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Tells us if the particle has exploded
  //----------------------------------------------------------------------------------------------------------------------
  bool           m_exploded = false;

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Explodes the firework, resets the particle properties, sets velocity to random spherical direction
  //----------------------------------------------------------------------------------------------------------------------
  void explode();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Calculates the colour of the particle for drawing
  //----------------------------------------------------------------------------------------------------------------------
  glm::vec4 calcCol( int const&_frame ) const;

}; // class end

#endif // FIREWORKPARTICLE_H
