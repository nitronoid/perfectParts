#ifndef SCENE_H
#define SCENE_H

#include <SDL.h>
#include <string>
#include <glm/glm.hpp>
#include "Emitter.h"
#include "ImGUIImpl.h"

//------------------------------------------------------------------------------------------------------------------------
/// \file Scene.h
/// \author Jack Diver
/// \version 5.2
/// \date Last Revision 03/05/17 Updated to NCCA coding standard \n

/// Revision History: See "https://github.com/nitronoid/perfectParts"
/// Initial Version 20/03/17

/// \class Scene
/// \brief encapsulates a 3D OpenGL Scene
/// \todo extend GUI functionality
//------------------------------------------------------------------------------------------------------------------------

class Scene
{
public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Flag to close the window
  //----------------------------------------------------------------------------------------------------------------------
  bool m_quit = false;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Holds the current user input
  //----------------------------------------------------------------------------------------------------------------------
  SDL_Event m_inputEvent;

public :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Default constructor that initialises member variables to default values
  //----------------------------------------------------------------------------------------------------------------------
  Scene() = default;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Constructor that takes a name, intial size and intial position to generate an SDL window,
  /// with an OpenGL context
  /// @param[in] _name the name of the window
  /// @param[in] _x the intial x position of the window
  /// @param[in] _y the intial y position of the window
  /// @param[in] _width the intial width of the window
  /// @param[in] _height the intial height of the window
  //----------------------------------------------------------------------------------------------------------------------
  Scene(std::string const&_name, int const&_x, int const&_y, int const&_width, int const&_height);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Delete copy constructor, DO NOT IMPLEMENT, emitter isn't trivially copyable
  //----------------------------------------------------------------------------------------------------------------------
  Scene( Scene const& ) = delete;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Delete assignment, DO NOT IMPLEMENT, emitter isn't trivially copyable
  //----------------------------------------------------------------------------------------------------------------------
  Scene &operator=( Scene const& ) = delete;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Default destructor
  //----------------------------------------------------------------------------------------------------------------------
  ~Scene();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Calls SDL_GL_MakeCurrent to link our SDL_Window and OpenGL context
  //----------------------------------------------------------------------------------------------------------------------
  inline void makeCurrent() const { SDL_GL_MakeCurrent(m_sdlWin,m_glContext); }
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws the GUI, particles and the window
  //----------------------------------------------------------------------------------------------------------------------
  void draw();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Handles user input, GUI, and manipulates emitter variables
  //----------------------------------------------------------------------------------------------------------------------
  void handleInput();

private :
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Flage to update the particle system
  //----------------------------------------------------------------------------------------------------------------------
  bool m_pause = false;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Flag to take a screen shot and save as a .png image
  //----------------------------------------------------------------------------------------------------------------------
  bool m_snap = false;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Flag to draw grid
  //----------------------------------------------------------------------------------------------------------------------
  bool m_grid = true;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Holds the ImGui inputs
  //----------------------------------------------------------------------------------------------------------------------
  ImGuiIO &m_io = ImGui::GetIO();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Stores a reference to the ImGui style
  //----------------------------------------------------------------------------------------------------------------------
  ImGuiStyle &m_style = ImGui::GetStyle();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Current GUI tab
  //----------------------------------------------------------------------------------------------------------------------
  int m_tab = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Width of the window
  //----------------------------------------------------------------------------------------------------------------------
  int m_width = 640;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Height of the window
  //----------------------------------------------------------------------------------------------------------------------
  int m_height = 480;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Window position
  //----------------------------------------------------------------------------------------------------------------------
  glm::ivec2 m_winPos = glm::ivec2(0.0f,0.0f);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Stores the mouse co-ordinates
  //----------------------------------------------------------------------------------------------------------------------
  glm::ivec2 m_mousePos;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Stores the translation of the opengl view
  //----------------------------------------------------------------------------------------------------------------------
  glm::vec3 m_translation = glm::vec3(0.0f,-30.0f,-150.0f);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Stores the rotation of the opengl view
  //----------------------------------------------------------------------------------------------------------------------
  glm::vec2 m_rotation = glm::vec2(0.0f,0.0f);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Name of the window
  //----------------------------------------------------------------------------------------------------------------------
  std::string m_name = "Default";
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The SDL Window
  //----------------------------------------------------------------------------------------------------------------------
  SDL_Window *m_sdlWin = nullptr;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief The particle Emitter
  //----------------------------------------------------------------------------------------------------------------------
  Emitter m_emit;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Stores the opengl context
  //----------------------------------------------------------------------------------------------------------------------
  SDL_GLContext m_glContext = NULL;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Holds our SDL Timer ID which uses the callback to update the scene
  //----------------------------------------------------------------------------------------------------------------------
  SDL_TimerID m_updateTimerID = 0;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Holds our SDL Mutex to lock our threads
  //----------------------------------------------------------------------------------------------------------------------
  SDL_mutex *m_mutex = nullptr;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief SDL condition which signals if we can draw
  //----------------------------------------------------------------------------------------------------------------------
  SDL_cond *m_canDraw = nullptr;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief SDL condition which signals if we can update
  //----------------------------------------------------------------------------------------------------------------------
  SDL_cond *m_canUpdate = nullptr;

private:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Our update call back which executes the update function
  //----------------------------------------------------------------------------------------------------------------------
  Uint32 updateCallback(Uint32 interval);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief A static timer callback which casts our update callback to allow the SDL TimerID to use it
  //----------------------------------------------------------------------------------------------------------------------
  static Uint32 timerCallback(Uint32 interval, void *param);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Initialises SDL and calls other initialisation functions
  //----------------------------------------------------------------------------------------------------------------------
  void init();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Initialises OpenGL to draw textured, point sprites with attenuation
  //----------------------------------------------------------------------------------------------------------------------
  void initGL() const;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Creates an OpenGL contex and links to our SDL window
  //----------------------------------------------------------------------------------------------------------------------
  void createGLContext();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Loads a projection matrix into OpenGL
  //----------------------------------------------------------------------------------------------------------------------
  void loadProjection(const glm::mat4 &_matrix) const;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Loads a model view matrix into OpenGL
  //----------------------------------------------------------------------------------------------------------------------
  void loadModelView(glm::mat4 const&_matrix) const;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Prints the provided string, and error caught by SDL, then exits
  //----------------------------------------------------------------------------------------------------------------------
  void ErrorExit(std::string const&_msg) const;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws a grid
  //----------------------------------------------------------------------------------------------------------------------
  void drawGrid(int const&_num, int const&_step) const;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Resets the OpenGL view to our default settings
  //----------------------------------------------------------------------------------------------------------------------
  void resetPos();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Converts mouse events to scene navigation
  //----------------------------------------------------------------------------------------------------------------------
  void handleMouse();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Resizes the OpenGL viewport, then loads a new projection based on window dimensions
  //----------------------------------------------------------------------------------------------------------------------
  void resize() const;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Reads the OpenGL pixel data and saves a .png file in /screenshots folder
  //----------------------------------------------------------------------------------------------------------------------
  void takeScreencap() const;
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Sets the style of the ImGui interface
  //----------------------------------------------------------------------------------------------------------------------
  void initStyle();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws all ImGui elements
  //----------------------------------------------------------------------------------------------------------------------
  void displayGui();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws the flame GUI
  //----------------------------------------------------------------------------------------------------------------------
  void displayFlameGui();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws the explosion GUI
  //----------------------------------------------------------------------------------------------------------------------
  void displayExplosionGui();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws the firework GUI
  //----------------------------------------------------------------------------------------------------------------------
  void displayFireworkGui();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws the system information GUI
  //----------------------------------------------------------------------------------------------------------------------
  void displaySystemGui();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Draws the gizmo within the scene and also transform information in the GUI
  //----------------------------------------------------------------------------------------------------------------------
  void transformView();

}; // class end

#endif // SCENE_H
