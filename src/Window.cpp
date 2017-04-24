#if defined (__linux__) || defined (WIN32)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#include "Window.h"
#include <string>

extern bool ColorSelector(const char* pLabel, glm::vec4& oRGBA);

Window::Window(const std::string &_name, int _x, int _y,int _width, int _height) :
  m_emit(glm::vec3(0.0f,0.0f,0.0f),50000)
{
  m_quit = false;
  m_pause = false;
  m_grid = true;
  m_name=_name;
  m_winPos.x = _x;
  m_winPos.y = _y;
  m_width=_width;
  m_height=_height;
  SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
  resetPos();
  init();
  m_io = ImGui::GetIO();
  m_io.Fonts->AddFontDefault();
  m_tab = 1;
  initStyle();
  m_emit.initTextures();
  m_drawing = false;
  m_updating = false;
//  m_updateTimerID = SDL_AddTimer(10, /*elapsed time in milliseconds*/
//                                 timerCallback, /*callback function*/
//                                 this /*pointer to the object*/);
}

Window::~Window()
{
  ImGui_ImplSdl_Shutdown();
  SDL_GL_DeleteContext(m_glContext);
  SDL_DestroyWindow(m_sdlWin);
  SDL_RemoveTimer(m_updateTimerID);
  SDL_Quit();
}

void Window::initStyle()
{
  m_style = ImGui::GetStyle();
  m_style.Colors[ImGuiCol_WindowBg] = ImVec4(1.0f,1.0f,1.0f,0.5f);
  m_style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f,1.0f,1.0f,0.75f);
  m_style.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f,1.0f,1.0f,0.6f);
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
  m_rotation = glm::vec2(0.0f,0.0f);
  m_translation = glm::vec2(0.0f,0.0f);
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
  ImGui_ImplSdl_Init(m_sdlWin);
}

void Window::initGL() const
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

void Window::resize() const
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

void Window::displayGui()
{
  ImGui_ImplSdl_ProcessEvent(&m_inputEvent);
  ImGui_ImplSdl_NewFrame(m_sdlWin);
  ImGui::Begin("Controls");
  if(ImGui::Button("Firework")) m_tab = 1;
  ImGui::SameLine();
  if(ImGui::Button("Flame")) m_tab = 2;
  ImGui::SameLine();
  if(ImGui::Button("Explosion")) m_tab = 3;
  ImGui::Separator();
  switch(m_tab)
  {
  case 1: {displayFireworkGui(); break;}
  case 2: {displayFlameGui(); break;}
  case 3: {displayExplosionGui(); break;}
  }
  ImGui::Separator();
  ImGui::Checkbox("Pause time",&m_pause);
  ImGui::SameLine();
  if(ImGui::Button("Clear System")) m_emit.clearParticles();
  //ImGui::SameLine();
//  ImVec4 col = {m_emit.m_fwCol.r,m_emit.m_fwCol.g,m_emit.m_fwCol.b,m_emit.m_fwCol.a};
//  ImGui::ColorEdit3("Firework colour", (float*)&col);
//  m_emit.m_fwCol = glm::vec4(col.x,col.y,col.z,col.w);

  ImGui::End();
}
void Window::displayFlameGui()
{
  ImGui::Checkbox("Ignite flame",&m_emit.m_flame);
}

void Window::displayExplosionGui()
{
  if(ImGui::Button("Detonate Explosion"))
  {
    m_emit.clearParticles();
    m_emit.m_explosion = 6;
  }
}

void Window::displayFireworkGui()
{
  ImGui::Text("Firework colour");
  ColorSelector("", m_emit.m_fwCol);
  ImGui::Checkbox("Blinking",&m_emit.m_fwBlink);
  if(ImGui::Button("Launch Firework"))
  {
    m_emit.m_firework = true;
  }
}

void Window::tick()
{

  if(!m_pause && !m_quit)
  {
    m_emit.update();
  }
  makeCurrent();
  while(SDL_PollEvent(&m_inputEvent))
  {
    displayGui();
    if(!m_io.WantCaptureMouse)
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
        case SDLK_e : m_emit.m_firework = true; break;
        case SDLK_a : m_emit.clearParticles();m_emit.m_explosion = 6; break;
        case SDLK_r : m_emit.m_flame = !m_emit.m_flame; break;
        case SDLK_t : std::cout<<m_emit.particleCount()<<'\n'; break;
        case SDLK_y : m_emit.clearParticles(); break;
        case SDLK_f : resetPos(); break;
        case SDLK_g : m_grid = !m_grid; break;
        default : break;
        }
      }
      case SDL_MOUSEMOTION : handleMouse(); break;
      default : break;
      }
    }
  }

}


void Window::handleMouse()
{
  const Uint8 *keystates = SDL_GetKeyboardState(NULL);
  glm::ivec2 newPos;
  float strength = 0.2f;
  Uint32 button = SDL_GetMouseState(&newPos.x, &newPos.y);
  glm::vec2 diff = ((glm::vec2)(newPos - m_mousePos)) * strength;

  switch(button)
  {
  case SDL_BUTTON_LMASK :
  {
    if(keystates[SDL_SCANCODE_LALT] || keystates[SDL_SCANCODE_RALT])
    {
      m_translation -= diff;
    }
    else
    {
      m_rotation.x -= diff.y;
      m_rotation.y += diff.x;
    }
    break;
  }
  case SDL_BUTTON_RMASK :
  {
    m_zoom -= diff.y;
    break;
  }
  case SDL_BUTTON_MMASK :
  {
    m_translation -= diff;
    break;
  }
  default: break;
  }
  m_mousePos = newPos;
}

void Window::ErrorExit(const std::string &_msg) const
{
  std::cerr<<_msg<<std::endl;
  std::cerr<<SDL_GetError()<<std::endl;
  SDL_Quit();
  exit(EXIT_FAILURE);
}


void Window::loadProjection(glm::mat4 _matrix) const
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrixf((const float*)glm::value_ptr(_matrix));
  glMatrixMode(GL_MODELVIEW);
}

void Window::loadModelView(glm::mat4 _matrix) const
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixf((const float*)glm::value_ptr(_matrix));
}

void Window::draw() const
{

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  loadModelView(glm::lookAt(glm::vec3(0,40,-150),glm::vec3(0,30,0),glm::vec3(0,1,0)));
  glTranslatef(0,0,-m_zoom);
  glTranslatef(m_translation.x,m_translation.y,0.0f);
  glRotatef(m_rotation.x,1.0f,0.0f,0.0f);
  glRotatef(m_rotation.y,0.0f,1.0f,0.0f);
  if(m_grid)
  {
    drawGrid(5,5);
  }
//  while(m_updating);
//  m_drawing = true;
  m_emit.draw();
//  m_drawing = false;
  ImGui::Render();
  SDL_GL_SwapWindow(m_sdlWin);
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
