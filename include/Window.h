#ifndef WINDOW_H__
#define WINDOW_H__
#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Emitter.h"

class Window
{
  //Public functions, constructors and destructor
public :
  Window(const std::string &_name, int _x, int _y, int _width, int _height);
  ~Window();

  inline void makeCurrent() { SDL_GL_MakeCurrent(m_sdlWin,m_glContext);}
  inline void swapWindow() { SDL_GL_SwapWindow(m_sdlWin); }
  void draw(const Emitter &_e);
  void pollEvent(Emitter &_e);

  //Private functions
private:
  void init();
  void initGL();
  void createGLContext();
  void loadProjection(glm::mat4 _matrix);
  void loadModelView(glm::mat4 _matrix);
  void ErrorExit(const std::string &_msg) const;
  void drawGrid() const;

  //Public members
public:
  bool m_quit=false;
  bool m_pause = false;
  bool m_trails = false;
  SDL_Event m_inputEvent;

  //Private members
private :
  int m_width;
  int m_height;
  int m_x;
  int m_y;
  int m_lastX;
  int m_lastY;
  bool m_mouseDown;
  std::string m_name;
  SDL_Window *m_sdlWin;
  SDL_GLContext m_glContext;

};

#endif
