#if defined (__linux__) || defined (WIN32)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#include "Scene.h"
#include <string>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

//-------------------------------------------------------------------------------------------------------------------------
/// @file Particle.cpp
/// @brief Implementation files for Particle class
//-------------------------------------------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------------------------------------------
extern bool ColorSelector(const char* pLabel, glm::vec4 &oRGBA); //foward declaration of colour picker
//-------------------------------------------------------------------------------------------------------------------------
Scene::Scene( std::string const&_name, int const&_x, int const&_y,int const&_width, int const&_height) :
  m_width(_width),
  m_height(_height),
  m_winPos(_x,_y),
  m_name(_name),
  m_emit(glm::vec3(0.0f,0.0f,0.0f),100000)
{
  //initialise mouse position
  SDL_GetMouseState(&m_mousePos.x, &m_mousePos.y);
  //set camera positions
  resetPos();
  //initialise SDL, OpenGL and ImGui
  init();
  //load textures
  m_emit.initTextures();
}
//-------------------------------------------------------------------------------------------------------------------------
Scene::~Scene()
{
  //shut down ImGui
  ImGui_ImplSdl_Shutdown();
  //Remove SDL and OpenGL
  SDL_GL_DeleteContext(m_glContext);
  SDL_DestroyWindow(m_sdlWin);
  SDL_Quit();
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::initStyle()
{
  //set the ImGui style
  /// The following section is modified from :-
  /// Omar Cornut (December 13, 2015). Simplified styling #438 [online].
  /// [Accessed 2017]. Available from: "https://github.com/ocornut/imgui/issues/438"

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

  ///end of citation
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::resetPos()
{
  m_rotation = glm::vec2(0.0f,0.0f);
  m_translation = glm::vec3(0.0f,-30.0f,150.0f);
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::init()
{
  //Initialise SDL
  if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
  {
    ErrorExit("Failed to initialise");
  }
  //create our window
  m_sdlWin=SDL_CreateWindow(m_name.c_str(),m_winPos.x,m_winPos.y,
                            m_width,m_height,
                            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE );
  //check this worked
  if(!m_sdlWin)
  {
    ErrorExit("Feailed to create Scene");
  }
  //create OpenGL context and init
  createGLContext();
  initGL();
  //init ImGui
  ImGui_ImplSdl_Init(m_sdlWin);
  initStyle();
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::initGL() const
{
  //point distance attenuation values
  float coeff[3] = {0.0f,0.1f,0.0f};
  float min = 1.0f;
  float thresh = 1.0f;
  // this sets the background colour
  glClearColor(0.0f,0.0f,0.0f,1.0f);
  //set viewport and load projection
  resize();
  //set OpenGL parameters
  glDisable(GL_LIGHTING);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glEnable( GL_POINT_SMOOTH ); //rounded points
  glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, coeff);
  glPointParameterfv(GL_POINT_SIZE_MIN, &min);
  glPointParameterfv(GL_POINT_FADE_THRESHOLD_SIZE, &thresh);
  glEnable(GL_TEXTURE_2D); //need this for textures
  glEnable(GL_POINT_SPRITE); //need this for point sprites
  glBlendFunc( GL_ONE, GL_ONE ); //additive blending
  glEnable( GL_BLEND );
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::resize() const
{
  //use passed values to load viewport
  glViewport(0,0,m_width,m_height);
  //load viewport projection
  loadProjection(glm::perspective(0.7f,float((float)m_width/m_height),0.1f,1000.0f));
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::createGLContext()
{
  //SDL GL SETTINGS
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,1);

  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS,1);
  SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES,4);

  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

  //Create the context
  m_glContext=SDL_GL_CreateContext(m_sdlWin);
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::displayGui()
{
  //proccess input in gui
  ImGui_ImplSdl_ProcessEvent(&m_inputEvent);
  ImGui_ImplSdl_NewFrame(m_sdlWin);
  //gui window
  if(ImGui::Begin("Controls"))
  {
    //control tabs
    ImGui::Tab(0,"Firework","Firework controls",&m_tab);
    ImGui::Tab(1,"Flame","Flame controls",&m_tab);
    ImGui::Tab(2,"Explosion","Explosion controls",&m_tab);
    ImGui::Tab(3,"System","System controls",&m_tab);
    ImGui::Separator();
    ImGui::NewLine();
    //display active tab gui
    switch (m_tab)
    {
    case 0: {displayFireworkGui(); break;}
    case 1: {displayFlameGui(); break;}
    case 2: {displayExplosionGui(); break;}
    case 3: {displaySystemGui(); break;}
    default: {break;}
    }
  }
  ImGui::End(); // end of window
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::displaySystemGui()
{
  //System GUI for performance stats and options
  ImGui::Checkbox("Pause time",&m_pause);
  ImGui::SameLine();
  ImGui::Checkbox("Display grid",&m_grid);
  if(ImGui::Button("Clear System")) m_emit.m_clear = true;
  if(ImGui::Button("Screenshot")) m_snap = true;
  ImGui::Text("Particle count: %zu / %zu",m_emit.particleCount(),m_emit.maxParticles());
  ImGui::Text("Frame time: %.3f ms/frame ", 1000.0f / ImGui::GetIO().Framerate);
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::displayFlameGui()
{
  //Flame GUI
  ImGui::Text("Fire colour");
  ColorSelector("Fire colour", m_emit.m_flCol);
  //when selected flame will be emitted
  ImGui::Checkbox("Ignite flame",&m_emit.m_flame);
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::displayExplosionGui()
{
  //Explosion GUI
  ImGui::Text("Explosion colour");
  ColorSelector("Explosion colour", m_emit.m_expCol);
  if(ImGui::Button("Detonate Explosion"))
  {
    //explode for 6 frames
    m_emit.m_explosion = 6;
  }
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::displayFireworkGui()
{
  //firework GUI
  ImGui::Text("Firework Colour");
  ColorSelector("Firework colour", m_emit.m_fwCol);

  ImGui::SliderAngle("Incline",&m_emit.m_fwIncline,-90.0f,90.0f);
  ImGui::SliderAngle("Rotation",&m_emit.m_fwRotation,0.0f,360.0f);
  ImGui::SliderFloat("Thrust",&m_emit.m_fwThrust,0.1f,2.0f);
  ImGui::SliderInt("Fuel",&m_emit.m_fwFuel,0,1000);
  ImGui::SliderInt("Fuse",&m_emit.m_fwFuse,2,200);
  ImGui::SliderInt("Tail",&m_emit.m_fwTrail,0,50);
  ImGui::SliderInt("Size",&m_emit.m_fwExpLife,0,200);
  ImGui::Checkbox("Blinking",&m_emit.m_fwBlink);
  if(ImGui::Button("Launch Firework"))
  {
    m_emit.m_firework = true;
  }
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::tick()
{
  //take a screenshot
  if(m_snap)
  {
    takeScreencap();
    m_snap = false;
  }
  //update
  if(!m_pause)
  {
    m_emit.update();
  }
  makeCurrent();
  //process user input
  while(SDL_PollEvent(&m_inputEvent))
  {
    //if the mouse isn't over the GUI process movement, and key press (need this for typing)
    if(!m_io.WantCaptureMouse)
    {
      //resize window
      if ((m_inputEvent.type == SDL_WINDOWEVENT) &&
          (m_inputEvent.window.event == SDL_WINDOWEVENT_RESIZED))
      {
        SDL_GetWindowSize(m_sdlWin,&m_width,&m_height);
        //prevent divide by zero
        if(m_height <= 0) { m_height = 1; }
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
        case SDLK_SPACE : m_pause = !m_pause; break;
        case SDLK_q : m_emit.m_firework = true; break;
        case SDLK_w : m_emit.m_explosion = 6; break;
        case SDLK_e : m_emit.m_flame = !m_emit.m_flame; break;
        case SDLK_c : m_emit.m_clear = true; break;
        case SDLK_f : resetPos(); break;
        case SDLK_g : m_grid = !m_grid; break;
        case SDLK_s : m_snap = true; break;
        default : break;
        }
      }
      //move scene based on mouse
      case SDL_MOUSEMOTION : handleMouse(); break;
      default : break;
      }
    }
  }
  //show GUI
  displayGui();

}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::handleMouse()
{
  //get keystates
  const Uint8 *keystates = SDL_GetKeyboardState(NULL);
  glm::ivec2 newPos;
  float strength = 0.2f;
  //get mouse state
  Uint32 button = SDL_GetMouseState(&newPos.x, &newPos.y);
  //calculate distance moved by mouse
  glm::vec2 diff = ((glm::vec2)(newPos - m_mousePos)) * strength;
  diff.x = -diff.x;

  switch(button)
  {
  case SDL_BUTTON_LMASK :
  {
    //translate from left mouse and alt key
    if(keystates[SDL_SCANCODE_LALT] || keystates[SDL_SCANCODE_RALT])
    {
      m_translation.x -= diff.x;
      m_translation.y -= diff.y;
    }
    else
    {
      //rotate from left mouse
      m_rotation.x += diff.y;
      m_rotation.y -= diff.x;
    }
    break;
  }
    //zoom from right mouse
  case SDL_BUTTON_RMASK :
  {
    m_translation.z -= diff.y;
    break;
  }
    //translate from middle mouse
  case SDL_BUTTON_MMASK :
  {
    m_translation.x -= diff.x;
    m_translation.y -= diff.y;
    break;
  }
  default: break;
  }
  //save mouse position
  m_mousePos = newPos;
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::ErrorExit(std::string const&_msg) const
{
  //print error and quit
  std::cerr<<_msg<<std::endl;
  std::cerr<<SDL_GetError()<<std::endl;
  SDL_Quit();
  exit(EXIT_FAILURE);
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::loadProjection(glm::mat4 const&_matrix) const
{
  //change to projection mode and load matrix
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMultMatrixf((const float*)glm::value_ptr(_matrix));
  //change back to modelview mode
  glMatrixMode(GL_MODELVIEW);
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::loadModelView(glm::mat4 const&_matrix) const
{
  //make sure we are in modelview mode
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glMultMatrixf((const float*)glm::value_ptr(_matrix));
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::draw()
{
  //clear the window
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //load our view
  glLoadIdentity();
  glTranslatef(m_translation.x,m_translation.y,-m_translation.z);
  glRotatef(m_rotation.x,1.0f,0.0f,0.0f);
  glRotatef(m_rotation.y,0.0f,1.0f,0.0f);
  //draw the grid
  if(m_grid)
  {
    drawGrid(5,5);
  }
  //draw particles
  m_emit.draw();
  //draw GUI
  ImGui::Render();
  //swap the newly drawn window
  SDL_GL_SwapWindow(m_sdlWin);
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::takeScreencap() const
{
  //get viewport dimensions
  int dimensions[4];
  glGetIntegerv(GL_VIEWPORT, dimensions);

  //allocate pixel memory
  uint8_t* pixels = new uint8_t[3 * dimensions[2] * dimensions[3]];

  //read data into allocated memory
  glReadPixels(0, 0, dimensions[2], dimensions[3], GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)pixels);

  //find current directory
  QDir snapDir(QDir::currentPath() + "/screenshots");

  //count number of existing screenshots
  int num = snapDir.count();

  //append number to the file path
  std::string fname = QDir::currentPath().toStdString() + "/screenshots/screencap_" + std::to_string(num-2) + ".png";

  //save the data as a .png image
  if (!save_png_libpng(fname.c_str(),pixels,dimensions[2],dimensions[3]))
  {
    ErrorExit("Failed to save image"); //if image saving failed, report error
  }
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::drawGrid( int const&_num, int const&_step) const
{
  //disable texturing
  glDisable(GL_TEXTURE_2D);
  //grid colour is white
  glColor4f(1.0f,1.0f,1.0f,1.0f);
  glLineWidth(1.0f);
  //draw lines
  glBegin(GL_LINES);
  for(int i = -_num*_step; i <= _num*_step; i+=_step)
  {
    glVertex3f(_num*_step,0.0f,i);
    glVertex3f(-_num*_step,0.0f,i);
    glVertex3f(i,0.0f,_num*_step);
    glVertex3f(i,0.0f,-_num*_step);
  }
  glEnd();
  //enable textyres again
  glEnable(GL_TEXTURE_2D);
}
//-------------------------------------------------------------------------------------------------------------------------
