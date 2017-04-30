#ifndef WINDOW_H__
#define WINDOW_H__
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Emitter.h"
#include "ImGUIImpl.h"

class Scene
{
  //Public functions, constructors and destructor
public :
  Scene(std::string const&_name, int const&_x, int const&_y, int const&_width, int const&_height);
  ~Scene();

  inline void makeCurrent() const { SDL_GL_MakeCurrent(m_sdlWin,m_glContext);}
  void draw() const;
  void tick();

  //Private functions
private:
  void init();
  void initGL() const;
  void createGLContext();
  void loadProjection(glm::mat4 _matrix) const;
  void loadModelView(glm::mat4 _matrix) const;
  void ErrorExit(std::string const&_msg) const;
  void drawGrid(int _num, int _step) const;
  void resetPos();
  void handleMouse();
  void resize() const;
  Uint32 timerCallback(Uint32 interval);
  static Uint32 timerCallback(Uint32 interval, void *param);

  //Gui functions
  void initStyle();
  void displayGui();
  void displayFlameGui();
  void displayExplosionGui();
  void displayFireworkGui();

  //Public members
public:
  bool m_quit=false;
  bool m_pause = false;
  SDL_Event m_inputEvent;

  //Private members
private :
  //Gui members
  ImGuiIO &m_io = ImGui::GetIO();
  ImGuiStyle &m_style = ImGui::GetStyle();

  //Window members
  int m_width = 640;
  int m_height = 480;
  glm::ivec2 m_winPos = glm::ivec2(0.0f,0.0f);
  glm::ivec2 m_mousePos;
  glm::vec2 m_translation = glm::vec2(0.0f,-30.0f);
  float m_zoom = 150.0f;
  glm::vec2 m_rotation = glm::vec2(0.0f,0.0f);
  std::string m_name = "Default";
  SDL_Window *m_sdlWin = NULL;
  Emitter m_emit;
  SDL_GLContext m_glContext;
  SDL_TimerID m_updateTimerID;
  bool m_grid = true;

  SDL_mutex *m_mutex = NULL;
  SDL_cond *m_canDraw = NULL;
  SDL_cond *m_canUpdate = NULL;

};

#endif
