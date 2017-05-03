#if defined (__linux__) || defined (WIN32)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#include "Scene.h"
#include <string>
#include <QDir>
#include <chrono>

extern bool ColorSelector(const char* pLabel, glm::vec4 &oRGBA);

Scene::Scene( std::string const&_name, int const&_x, int const&_y,int const&_width, int const&_height) :
  m_width(_width),
  m_height(_height),
  m_winPos(_x,_y),
  m_name(_name),
  m_emit(glm::vec3(0.0f,0.0f,0.0f),100000)
{
  SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
  resetPos();
  init();
  m_emit.initTextures();
  //  m_mutex = SDL_CreateMutex();
  //  m_canDraw = SDL_CreateCond();
  //  m_canUpdate = SDL_CreateCond();
  //  m_updateTimerID = SDL_AddTimer(10, /*elapsed time in milliseconds*/
  //                                 timerCallback, /*callback function*/
  //                                 this /*pointer to the object*/);
}

Scene::~Scene()
{
  ImGui_ImplSdl_Shutdown();
  SDL_RemoveTimer(m_updateTimerID);
  //  SDL_DestroyMutex(m_mutex);
  //  SDL_DestroyCond(m_canDraw);
  //  SDL_DestroyCond(m_canUpdate);
  SDL_GL_DeleteContext(m_glContext);
  SDL_DestroyWindow(m_sdlWin);
  SDL_Quit();
}

void Scene::initStyle()
{
  ImVec4 col_text = ImColor::HSV(0.56f, 0.079f, 0.922f);
  ImVec4 col_main = ImColor::HSV(0.56f, 0.706f, 0.631f);
  ImVec4 col_back = ImColor::HSV(0.56f, 0.231f, 0.157f);
  ImVec4 col_area = ImColor::HSV(0.56f, 0.486f, 0.392f);

  m_style = ImGui::GetStyle();

  m_style.Colors[ImGuiCol_Text]                  = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
  m_style.Colors[ImGuiCol_TextDisabled]          = ImVec4(col_text.x, col_text.y, col_text.z, 0.58f);
  m_style.Colors[ImGuiCol_WindowBg]              = ImVec4(col_back.x, col_back.y, col_back.z, 1.00f);
  m_style.Colors[ImGuiCol_ChildWindowBg]         = ImVec4(col_area.x, col_area.y, col_area.z, 0.00f);
  m_style.Colors[ImGuiCol_Border]                = ImVec4(col_text.x, col_text.y, col_text.z, 0.30f);
  m_style.Colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
  m_style.Colors[ImGuiCol_FrameBg]               = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
  m_style.Colors[ImGuiCol_FrameBgHovered]        = ImVec4(col_main.x, col_main.y, col_main.z, 0.68f);
  m_style.Colors[ImGuiCol_FrameBgActive]         = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
  m_style.Colors[ImGuiCol_TitleBg]               = ImVec4(col_main.x, col_main.y, col_main.z, 0.45f);
  m_style.Colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(col_main.x, col_main.y, col_main.z, 0.35f);
  m_style.Colors[ImGuiCol_TitleBgActive]         = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
  m_style.Colors[ImGuiCol_MenuBarBg]             = ImVec4(col_area.x, col_area.y, col_area.z, 0.57f);
  m_style.Colors[ImGuiCol_ScrollbarBg]           = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
  m_style.Colors[ImGuiCol_ScrollbarGrab]         = ImVec4(col_main.x, col_main.y, col_main.z, 0.31f);
  m_style.Colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
  m_style.Colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
  m_style.Colors[ImGuiCol_ComboBg]               = ImVec4(col_area.x, col_area.y, col_area.z, 1.00f);
  m_style.Colors[ImGuiCol_CheckMark]             = ImVec4(col_main.x, col_main.y, col_main.z, 0.80f);
  m_style.Colors[ImGuiCol_SliderGrab]            = ImVec4(col_main.x, col_main.y, col_main.z, 0.24f);
  m_style.Colors[ImGuiCol_SliderGrabActive]      = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
  m_style.Colors[ImGuiCol_Button]                = ImVec4(col_main.x, col_main.y, col_main.z, 0.44f);
  m_style.Colors[ImGuiCol_ButtonHovered]         = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
  m_style.Colors[ImGuiCol_ButtonActive]          = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
  m_style.Colors[ImGuiCol_Header]                = ImVec4(col_main.x, col_main.y, col_main.z, 0.76f);
  m_style.Colors[ImGuiCol_HeaderHovered]         = ImVec4(col_main.x, col_main.y, col_main.z, 0.86f);
  m_style.Colors[ImGuiCol_HeaderActive]          = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
  m_style.Colors[ImGuiCol_Column]                = ImVec4(col_text.x, col_text.y, col_text.z, 0.32f);
  m_style.Colors[ImGuiCol_ColumnHovered]         = ImVec4(col_text.x, col_text.y, col_text.z, 0.78f);
  m_style.Colors[ImGuiCol_ColumnActive]          = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
  m_style.Colors[ImGuiCol_ResizeGrip]            = ImVec4(col_main.x, col_main.y, col_main.z, 0.20f);
  m_style.Colors[ImGuiCol_ResizeGripHovered]     = ImVec4(col_main.x, col_main.y, col_main.z, 0.78f);
  m_style.Colors[ImGuiCol_ResizeGripActive]      = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
  m_style.Colors[ImGuiCol_CloseButton]           = ImVec4(col_text.x, col_text.y, col_text.z, 0.16f);
  m_style.Colors[ImGuiCol_CloseButtonHovered]    = ImVec4(col_text.x, col_text.y, col_text.z, 0.39f);
  m_style.Colors[ImGuiCol_CloseButtonActive]     = ImVec4(col_text.x, col_text.y, col_text.z, 1.00f);
  m_style.Colors[ImGuiCol_PlotLines]             = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
  m_style.Colors[ImGuiCol_PlotLinesHovered]      = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
  m_style.Colors[ImGuiCol_PlotHistogram]         = ImVec4(col_text.x, col_text.y, col_text.z, 0.63f);
  m_style.Colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(col_main.x, col_main.y, col_main.z, 1.00f);
  m_style.Colors[ImGuiCol_TextSelectedBg]        = ImVec4(col_main.x, col_main.y, col_main.z, 0.43f);
  m_style.Colors[ImGuiCol_ModalWindowDarkening]  = ImVec4(0.20f, 0.20f, 0.20f, 0.35f);

  m_style.WindowRounding = 0.0f;
  m_style.Alpha = 0.75f;
}

Uint32 Scene::timerCallback(Uint32 interval)
{
  if(!m_pause && !m_quit)
  {
    SDL_LockMutex(m_mutex);
    while(!m_canUpdate)
    {
      SDL_CondWait(m_canUpdate, m_mutex);
    }
    m_emit.update();
    SDL_UnlockMutex(m_mutex);
    SDL_CondSignal(m_canDraw);
  }
  return interval;
}

Uint32 Scene::timerCallback(Uint32 interval, void * param)
{
  return ((Scene*)param)->timerCallback(interval);
}

void Scene::resetPos()
{
  m_rotation = glm::vec2(0.0f,0.0f);
  m_translation = glm::vec3(0.0f,-30.0f,150.0f);
}

void Scene::init()
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
    ErrorExit("Feailed to create Scene");
  }

  createGLContext();
  initGL();
  ImGui_ImplSdl_Init(m_sdlWin);
  initStyle();
}

void Scene::initGL() const
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

void Scene::resize() const
{
  glViewport(0,0,m_width,m_height);
  loadProjection(glm::perspective(0.7f,float((float)m_width/m_height),0.1f,1000.0f));
}

void Scene::createGLContext()
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

void Scene::displayGui()
{
  ImGui_ImplSdl_ProcessEvent(&m_inputEvent);
  ImGui_ImplSdl_NewFrame(m_sdlWin);
  if(ImGui::Begin("Controls"))
  {
    ImGui::Tab(0,"Firework","Firework controls",&m_tab);
    ImGui::Tab(1,"Flame","Flame controls",&m_tab);
    ImGui::Tab(2,"Explosion","Explosion controls",&m_tab);
    ImGui::Tab(3,"System","System controls",&m_tab);
    ImGui::Separator();
    ImGui::NewLine();

    switch (m_tab)
    {
    case 0: {displayFireworkGui(); break;}
    case 1: {displayFlameGui(); break;}
    case 2: {displayExplosionGui(); break;}
    case 3: {displaySystem(); break;}
    default: {break;}
    }
  }
  ImGui::End();
}

void Scene::displaySystem()
{
  ImGui::Checkbox("Pause time",&m_pause);
  ImGui::SameLine();
  ImGui::Checkbox("Display grid",&m_grid);
  if(ImGui::Button("Clear System")) m_emit.m_clear = true;
  if(ImGui::Button("Screenshot")) m_snap = true;
  ImGui::Text("Particle count: %zu / %zu",m_emit.particleCount(),m_emit.maxParticles());
  ImGui::Text("Frame time: %.3f ms/frame ", 1000.0f / ImGui::GetIO().Framerate);
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
}

void Scene::displayFlameGui()
{
  ImGui::Text("Fire colour");
  ColorSelector("Fire colour", m_emit.m_fiCol);
  ImGui::Checkbox("Ignite flame",&m_emit.m_flame);
}

void Scene::displayExplosionGui()
{
  ImGui::Text("Explosion colour");
  ColorSelector("Explosion colour", m_emit.m_expCol);
  if(ImGui::Button("Detonate Explosion"))
  {
    m_emit.m_explosion = 6;
  }
}

void Scene::displayFireworkGui()
{
  ImGui::Text("Firework Colour");
  ColorSelector("Firework colour", m_emit.m_fwCol);

  ImGui::SliderAngle("Steepness",&m_emit.m_fwPhi,-90.0f,90.0f);
  ImGui::SliderAngle("Rotation",&m_emit.m_fwTheta,0.0f,360.0f);
  ImGui::SliderFloat("Thrust",&m_emit.m_fwThrust,0.1f,2.0f);
  ImGui::SliderInt("Fuel",&m_emit.m_fwFuel,0,1000);
  ImGui::SliderInt("Fuse",&m_emit.m_fwFuse,2,200);
  ImGui::SliderInt("Trail length",&m_emit.m_fwTrail,0,50);
  ImGui::SliderInt("Explosion size",&m_emit.m_fwExpLife,0,200);
  ImGui::Checkbox("Blinking",&m_emit.m_fwBlink);
  if(ImGui::Button("Launch Firework"))
  {
    m_emit.m_firework = true;
  }
}

void Scene::tick()
{
  if(m_snap)
  {
    takeScreencap();
    m_snap = false;
  }
  if(!m_pause && !m_quit)
  {
//    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    m_emit.update();
//    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
//    auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
//    std::cout <<"update func: "<< duration<<'\n';
  }
  makeCurrent();
  while(SDL_PollEvent(&m_inputEvent))
  {

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
        case SDLK_a : m_emit.m_explosion = 6; break;
        case SDLK_r : m_emit.m_flame = !m_emit.m_flame; break;
        case SDLK_t : std::cout<<m_emit.particleCount()<<'\n'; break;
        case SDLK_y : m_emit.m_clear = true; break;
        case SDLK_f : resetPos(); break;
        case SDLK_g : m_grid = !m_grid; break;
        case SDLK_0 : m_snap = true; break;
        default : break;
        }
      }
      case SDL_MOUSEMOTION : handleMouse(); break;
      default : break;
      }
    }
  }
  displayGui();

}

void Scene::handleMouse()
{
  const Uint8 *keystates = SDL_GetKeyboardState(NULL);
  glm::ivec2 newPos;
  float strength = 0.2f;
  Uint32 button = SDL_GetMouseState(&newPos.x, &newPos.y);
  glm::vec2 diff = ((glm::vec2)(newPos - m_mousePos)) * strength;
  diff.x = -diff.x;

  switch(button)
  {
  case SDL_BUTTON_LMASK :
  {
    if(keystates[SDL_SCANCODE_LALT] || keystates[SDL_SCANCODE_RALT])
    {
      m_translation.x -= diff.x;
      m_translation.y -= diff.y;
    }
    else
    {
      m_rotation.x += diff.y;
      m_rotation.y -= diff.x;
    }
    break;
  }
  case SDL_BUTTON_RMASK :
  {
    m_translation.z -= diff.y;
    break;
  }
  case SDL_BUTTON_MMASK :
  {
    m_translation.x -= diff.x;
    m_translation.y -= diff.y;
    break;
  }
  default: break;
  }
  m_mousePos = newPos;
}

void Scene::ErrorExit(std::string const&_msg) const
{
  std::cerr<<_msg<<std::endl;
  std::cerr<<SDL_GetError()<<std::endl;
  SDL_Quit();
  exit(EXIT_FAILURE);
}

void Scene::loadProjection(glm::mat4 _matrix) const
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrixf((const float*)glm::value_ptr(_matrix));
  glMatrixMode(GL_MODELVIEW);
}

void Scene::loadModelView(glm::mat4 _matrix) const
{
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixf((const float*)glm::value_ptr(_matrix));
}

void Scene::draw()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  glTranslatef(m_translation.x,m_translation.y,-m_translation.z);
  glRotatef(m_rotation.x,1.0f,0.0f,0.0f);
  glRotatef(m_rotation.y,0.0f,1.0f,0.0f);
  if(m_grid)
  {
    drawGrid(5,5);
  }

  //  SDL_LockMutex(m_mutex);
  //  while(!m_canDraw)
  //  {
  //    SDL_CondWait(m_canDraw, m_mutex);
  //  }
//  std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
  m_emit.draw();
//  std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
//  auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
  //std::cout <<"draw func: "<< duration<<'\n';
  //  SDL_UnlockMutex(m_mutex);
  //  SDL_CondSignal(m_canUpdate);

  ImGui::Render();
  SDL_GL_SwapWindow(m_sdlWin);
}

void Scene::takeScreencap() const
{
  //get viewport dimensions
  int dimensions[4];
  glGetIntegerv(GL_VIEWPORT, dimensions);

  //allocate pixel memory
  uint8_t* pixels = new uint8_t[3 * dimensions[2] * dimensions[3]];

  //read data into allocated memory
  glReadPixels(0, 0, dimensions[2], dimensions[3], GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)pixels);

  //save the data as a .png image
  QDir snapDir(QDir::currentPath() + "/screenshots");
  int num = snapDir.count();
  std::string fname = QDir::currentPath().toStdString() + "/screenshots/screencap_" + std::to_string(num-2) + ".png";
  if (!save_png_libpng(fname.c_str(),pixels,dimensions[2],dimensions[3]))
  {
    ErrorExit("Failed to save image"); //if image saving failed, report error
  }
}

void Scene::drawGrid( int const&_num, int const&_step) const
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
