#if defined (__linux__) || defined (WIN32)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#include "Window.h"

Window::Window(const std::string &_name, int _x, int _y,int _width, int _height) : m_emit(glm::vec3(0.0f,0.0f,0.0f),5000)
{
  m_quit = false;
  m_pause = false;
  m_name=_name;
  m_winPos.x = _x;
  m_winPos.y = _y;
  m_width=_width;
  m_height=_height;
  SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
  resetPos();
  init();
  m_emit.initTextures();
  m_drawing = false;
  m_updating = false;
  m_updateTimerID = SDL_AddTimer(10, /*elapsed time in milliseconds*/
                                 timerCallback, /*callback function*/
                                 this /*pointer to the object*/);
}

Window::~Window()
{
  SDL_DestroyWindow(m_sdlWin);
  SDL_RemoveTimer(m_updateTimerID);
  SDL_Quit();
}

Uint32 Window::timerCallback(Uint32 interval)
{
  if(!m_pause && !m_quit)
  {
    while(m_drawing);
    m_updating = true;
    m_emit.update();
    m_updating = false;
  }
  return interval;
}

Uint32 Window::timerCallback(Uint32 interval, void * param)
{
  return ((Window*)param)->timerCallback(interval);
}

void Window::resetPos()
{
  m_rotation.x = 0.0f;
  m_rotation.y = 0.001f;
  m_translation.x = 0.0f;
  m_translation.y = 0.0f;
  m_zoom = 0.0f;
}

void Window::init()
{
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    ErrorExit("Failed to initialise");
  }

  m_sdlWin=SDL_CreateWindow(m_name.c_str(),m_winPos.x,m_winPos.y,
                            m_width,m_height,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
  if(!m_sdlWin)
  {
    ErrorExit("Feailed to create Window");
  }

  createGLContext();
  initGL();
}

void Window::initGL()
{
  float coeff[3] = {0.0f,0.1f,0.0f};
  float min = 1.0f;
  float thresh = 1.0f;
  // this sets the background colour
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  // this is how big our window is for drawing
  resize();
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable( GL_POINT_SMOOTH );
  glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, coeff);
  glPointParameterfv(GL_POINT_SIZE_MIN, &min);
  glPointParameterfv(GL_POINT_FADE_THRESHOLD_SIZE, &thresh);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_POINT_SPRITE);
  glBlendFunc( GL_ONE, GL_ONE );
  glEnable( GL_BLEND );
}

void Window::resize()
{
  glViewport(0,0,m_width,m_height);
  loadProjection(glm::perspective(0.7f,float((float)m_width/m_height),0.1f,1000.0f));
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

void Window::pollEvent()
{
//  if(!m_pause && !m_quit)
//  {
//    m_emit.update();
//  }
  makeCurrent();
  while(SDL_PollEvent(&m_inputEvent))
  {
    if ((m_inputEvent.type == SDL_WINDOWEVENT) &&
        (m_inputEvent.window.event == SDL_WINDOWEVENT_RESIZED))
    {
      SDL_GetWindowSize(m_sdlWin,&m_width,&m_height);
      resize();
    }
    switch (m_inputEvent.type)
    {
    case SDL_QUIT : m_quit = true; break;
    case SDL_KEYDOWN:
    {
      switch( m_inputEvent.key.keysym.sym )
      {
      case SDLK_ESCAPE :  m_quit = true; break;
      case SDLK_w : m_pause = !m_pause; break;
      case SDLK_e : m_emit.createFirework(); break;
      case SDLK_r : m_emit.m_flame = !m_emit.m_flame; break;
      case SDLK_f : resetPos(); break;
      default : break;
      }
    }
    case SDL_MOUSEMOTION : handleMouse(); break;
    default : break;
    }
  }
}


void Window::handleMouse()
{
  const Uint8 *keystates = SDL_GetKeyboardState(NULL);
  int x, y;
  float strength = 0.2f;
  Uint32 button = SDL_GetMouseState(&x, &y);
  float diffX = ((float)(x - m_mousePos.x) * strength);
  float diffY = ((float)(y - m_mousePos.y) * strength);

  switch(button)
  {
  case SDL_BUTTON_LMASK :
  {
    if(keystates[SDL_SCANCODE_LALT] || keystates[SDL_SCANCODE_RALT])
    {
      m_translation.x -= diffX;
      m_translation.y -= diffY;
    }
    else
    {
      m_rotation.x += diffY;
      m_rotation.y += diffX;
    }
    break;
  }
  case SDL_BUTTON_RMASK :
  {
    m_zoom -= diffY;
    break;
  }
  case SDL_BUTTON_MMASK :
  {
    m_translation.x -= diffX;
    m_translation.y -= diffY;
  }
  default: break;
  }
  m_mousePos.x = x;
  m_mousePos.y = y;
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

void Window::draw()
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  loadModelView(glm::lookAt(glm::vec3(0,40,-150),glm::vec3(0,30,0),glm::vec3(0,1,0)));
  glTranslatef(0,0,-m_zoom);
  glTranslatef(m_translation.x,m_translation.y,0.0f);
  glRotatef(m_rotation.x,1.0f,0.0f,0.0f);
  glRotatef(m_rotation.y,0.0f,1.0f,0.0f);
  drawGrid(5,5);
  while(m_updating);
  m_drawing = true;
  m_emit.draw();
  m_drawing = false;
}

void Window::drawGrid(int _num, int _step) const
{
  glDisable(GL_TEXTURE_2D);
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  glLineWidth(1.0f);
  glBegin(GL_LINES);
  for(int i = -_num*_step; i <= _num*_step; i+=_step)
  {
    glVertex3f(_num*_step,0.0f,i);
    glVertex3f(-_num*_step,0.0f,i);
    glVertex3f(i,0.0f,_num*_step);
    glVertex3f(i,0.0f,-_num*_step);
  }
  glEnd();
  glEnable(GL_TEXTURE_2D);
}



