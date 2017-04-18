#ifndef WINDOW_H__
#define WINDOW_H__
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Emitter.h"
#include "Camera.h"

class Window
{
  //Public functions, constructors and destructor
public :
  Window(const std::string &_name, int _x, int _y, int _width, int _height);
  ~Window();

  inline void makeCurrent() { SDL_GL_MakeCurrent(m_sdlWin,m_glContext);}
  inline void swapWindow() { SDL_GL_SwapWindow(m_sdlWin); }
  void draw();
  void pollEvent();

  //Private functions
private:
  void init();
  void initGL();
  void createGLContext();
  void loadProjection(glm::mat4 _matrix);
  void loadModelView(glm::mat4 _matrix);
  void ErrorExit(const std::string &_msg) const;
  void drawGrid(int _num, int _step) const;
  void resetPos();
  void handleMouse();
  void resize();
  Uint32 timerCallback(Uint32 interval);
  static Uint32 timerCallback(Uint32 interval, void *param);

  //Public members
public:
  bool m_quit=false;
  bool m_pause = false;
  SDL_Event m_inputEvent;

  //Private members
private :
  Emitter m_emit;
  int m_width;
  int m_height;
  glm::ivec2 m_winPos;
  glm::ivec2 m_mousePos;
  glm::vec2 m_translation;
  float m_zoom;
  glm::vec2 m_rotation;
  std::string m_name;
  SDL_Window *m_sdlWin;
  SDL_GLContext m_glContext;
  SDL_TimerID m_updateTimerID;
  bool m_drawing;
  bool m_updating;

};

#endif
