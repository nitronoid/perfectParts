#if defined (__linux__) || defined (WIN32)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#include "Window.h"

Window::Window(const std::string &_name, int _x, int _y,int _width, int _height)
{
  m_name=_name;
  m_x=_x;
  m_y=_y;
  m_width=_width;
  m_height=_height;
  init();
}

Window::~Window()
{
  SDL_DestroyWindow(m_sdlWin);
  SDL_Quit();
}

void Window::init()
{
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    ErrorExit("Failed to initialise");
  }

  m_sdlWin=SDL_CreateWindow(m_name.c_str(),m_x,m_y,
                            m_width,m_height,
                            SDL_WINDOW_OPENGL );
  if(!m_sdlWin)
  {
    ErrorExit("Failed to create Window");
  }

  createGLContext();
  initGL();
}

void Window::initGL()
{
  // this sets the background colour
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  // this is how big our window is for drawing
  glViewport(0,0,720,576);

  glm::mat4 view = glm::perspective(0.7f,float(720/576),0.1f,1000.0f);
  loadProjection(view);
  view = glm::lookAt(glm::vec3(0,50,-150),glm::vec3(0,50,0),glm::vec3(0,1,0));
  loadModelView(view);
  glDisable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable( GL_POINT_SMOOTH );
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_POINT_SPRITE);
  glBlendFunc( GL_ONE, GL_ONE );
  glEnable( GL_BLEND );


}

void Window::createGLContext()
{
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

  m_glContext=SDL_GL_CreateContext(m_sdlWin);

}

void Window::pollEvent(Emitter &_e)
{
  makeCurrent();
  SDL_PollEvent(&m_inputEvent);
  switch (m_inputEvent.type)
  {
  case SDL_QUIT : m_quit = true; break;
  case SDL_KEYDOWN:
  {
    switch( m_inputEvent.key.keysym.sym )
    {
    case SDLK_ESCAPE :  m_quit = true; break;
    case SDLK_w : m_pause = !m_pause; break;
    case SDLK_s : m_trails = !m_trails; break;
    case SDLK_e :
    {
      _e.createFirework();
      break;
    }
    case SDLK_r :
    {
      _e.m_smoke = !_e.m_smoke;
      break;
    }
    default : break;
    }
  }
  default : break;
  }
}

void Window::ErrorExit(const std::string &_msg) const
{
  std::cerr<<_msg<<std::endl;
  std::cerr<<SDL_GetError()<<std::endl;
  SDL_Quit();
  exit(EXIT_FAILURE);
}


void Window::loadProjection(glm::mat4 _matrix)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrixf((const float*)glm::value_ptr(_matrix));
  glMatrixMode(GL_MODELVIEW);
}

void Window::loadModelView(glm::mat4 _matrix)
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixf((const float*)glm::value_ptr(_matrix));
}

void Window::draw(const Emitter &_e)
{
  _e.draw();
}



