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
  m_winPos.x = _x;
  m_winPos.y = _y;
  m_width=_width;
  m_height=_height;
  SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
  m_rotation.x = 0.0f;
  m_rotation.y = 0.001f;
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

  m_sdlWin=SDL_CreateWindow(m_name.c_str(),m_winPos.x,m_winPos.y,
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

  glLoadIdentity();
  glm::mat4 view = glm::perspective(0.7f,float(720/576),0.1f,1000.0f);
  loadProjection(view);
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
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
  while(SDL_PollEvent(&m_inputEvent))
  {
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
    case SDL_MOUSEMOTION :
    {
      int x, y;
      if(SDL_GetGlobalMouseState( &x, &y ) == SDL_BUTTON_LMASK)
      {
//        //make sure we transform the correct matrix
//        glMatrixMode(GL_MODELVIEW);
//        //obtain the current view matrix
//        GLfloat view[16];
//        glGetFloatv(GL_MODELVIEW_MATRIX, view);
//        //get the eye vector from view matrix
//        glm::vec3 eye = glm::normalize(glm::vec3(view[2],view[6],view[10]));
//        //calculate the cross product of this with Y axis
//        glm::vec3 axis = glm::cross(eye,glm::vec3(0.0f,1.0f,0.0f));

//        //rotate around y
//        glRotatef( ((float)(x - m_mousePos.x) * 0.1f), 0.0f,1.0f,0.0f );
//        //rotate around cross product
//        glRotatef( m_flip * ((float)(y - m_mousePos.y) * 0.1f), axis.x,axis.y,axis.z );


//        glGetFloatv(GL_MODELVIEW_MATRIX, view);
//        eye = glm::normalize(glm::vec3(view[2],view[6],view[10]));
//        if(eye.y == 1.0f || eye.y == -1.0f)
//        {
//          glRotatef( -m_flip * ((float)(y - m_mousePos.y) * 0.1f), 1.0f,0.0f,0.0f );
//          m_flip = -m_flip;
//        }

//        glRotatef( ((float)(x - m_mousePos.x) * 0.1f), 0.0f,1.0f,0.0f );
//        //rotate around cross product
//        glRotatef( ((float)(y - m_mousePos.y) * 0.1f), 1.0f,0.0f,0.0f );
        m_rotation.x += ((float)(y - m_mousePos.y) * 0.1f);
        m_rotation.y += ((float)(x - m_mousePos.x) * 0.1f);
      }
      m_mousePos.x = x;
      m_mousePos.y = y;
      break;
    }
    default : break;
    }
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
  glLoadIdentity();
  glm::mat4 view = glm::lookAt(glm::vec3(0,50,-150),glm::vec3(0,50,0),glm::vec3(0,1,0));
  loadModelView(view);
  glRotatef(m_rotation.x,1.0f,0.0f,0.0f);
  glRotatef(m_rotation.y,0.0f,1.0f,0.0f);
  drawGrid();
  _e.draw();

}

void Window::drawGrid() const
{
  glDisable(GL_TEXTURE_2D);
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  glLineWidth(1.0f);
  glBegin(GL_LINES);
  int num = 20;
  int step = 4;
  for(int i = -num; i <= num; i+=step)
  {
    glVertex3f(num,0.0f,i);
    glVertex3f(-num,0.0f,i);
    glVertex3f(i,0.0f,num);
    glVertex3f(i,0.0f,-num);
  }
  glEnd();
  glEnable(GL_TEXTURE_2D);
}



