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
#include "ImGuizmo.h"

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
  //mutex and conditions to prevent two threads from accessing the particle data at the same time
  m_mutex = SDL_CreateMutex();
  m_canDraw = SDL_CreateCond();
  m_canUpdate = SDL_CreateCond();
  //Use an SDL Timer to update the system regularly in a seperate thread
  m_updateTimerID = SDL_AddTimer(15, /*elapsed time in milliseconds*/
                                 timerCallback, /*callback function*/
                                 this /*pointer to the object*/);
}
//-------------------------------------------------------------------------------------------------------------------------
Scene::~Scene()
{
  //shut down ImGui
  ImGui_ImplSdl_Shutdown();
  //remove mutex and conditions
  SDL_DestroyMutex(m_mutex);
  SDL_DestroyCond(m_canDraw);
  SDL_DestroyCond(m_canUpdate);
  //remove our SDL Timer
  SDL_RemoveTimer(m_updateTimerID);
  //Remove SDL and OpenGL
  SDL_GL_DeleteContext(m_glContext);
  SDL_DestroyWindow(m_sdlWin);
  SDL_Quit();
}
//-------------------------------------------------------------------------------------------------------------------------
Uint32 Scene::updateCallback(Uint32 interval)
{
  //Use mutex's to ensure we don't interfere with drawing
  if(!m_pause && !m_quit) //if the scene isn't paused
  {
    SDL_LockMutex(m_mutex);
    while(!m_canUpdate)
    {
      SDL_CondWait(m_canUpdate, m_mutex);
    }
    //update the particle system
    m_emit.update();
    //allow drawing to resume
    SDL_UnlockMutex(m_mutex);
    SDL_CondSignal(m_canDraw);
  }
  return interval;
}
//-------------------------------------------------------------------------------------------------------------------------
Uint32 Scene::timerCallback(Uint32 interval, void * param)
{
  //use this static function to cast the pointer to our actual function to execute
  return ((Scene*)param)->updateCallback(interval);
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

  //load current style
  m_style = ImGui::GetStyle();
  //modify the style colours
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

  //Sharp window edges and semi transparent GUI
  m_style.WindowRounding = 10.0f;
  m_style.Alpha = 0.75f;

  ///end of citation
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::resetPos()
{
  //default rotation and translation
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
  ImGuizmo::BeginFrame();
  //gui window
  if(ImGui::Begin("Controls"))
  {
    //control tabs
    ImGui::Tab(0,"Firework","Firework controls",&m_tab);
    ImGui::Tab(1,"Flame","Flame controls",&m_tab);
    ImGui::Tab(2,"Explosion","Explosion controls",&m_tab);
    ImGui::Tab(3,"System","System controls",&m_tab);
    ImGui::Tab(4,"View","View controls",&m_tab);
    ImGui::Separator();
    ImGui::NewLine();
    //display active tab gui
    switch (m_tab)
    {
    case 0: {displayFireworkGui(); break;}
    case 1: {displayFlameGui(); break;}
    case 2: {displayExplosionGui(); break;}
    case 3: {displaySystemGui(); break;}
    case 4: {transformView();break;}
    default: {break;}
    }
  }
  ImGui::End(); // end of window

}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::transformView()
{
  //Scene position and rotation
  ImGui::Text("Scene transform");
  ImGui::SliderFloat("X rotation",&m_rotation.x,0.0f,359.999f);
  ImGui::SliderFloat("Y rotation",&m_rotation.y,0.0f,359.999f);
  ImGui::SliderFloat3("Position",glm::value_ptr(m_translation),-1000.0f,1000.0f);
  ImGui::Separator();

  /// The following section is modified from :-
  /// Omar Cornut (April 4, 2017). Immediate mode 3D gizmo for scene editing [online].
  /// [Accessed 2017]. Available from: "https://github.com/CedricGuillemet/ImGuizmo"

  ImGui::Text("Object transform");
  //get current keys being pressed
  const Uint8 *keystates = SDL_GetKeyboardState(NULL);
  //get the current projection and modelview matrices
  float mv[16];
  float pm[16];
  glGetFloatv(GL_MODELVIEW_MATRIX, mv);
  glGetFloatv(GL_PROJECTION_MATRIX, pm);
  //convert our 4x4 Matrix to a float array that ImGuizmo can manipulate
  float *matrix = new float[16];
  matrix = (float*)glm::value_ptr(m_emit.m_transform);

  //Declare the current operation and mode
  static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
  static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);

  //Buttons to select current operation
  if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
  {
    mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
  }
  ImGui::SameLine(); //buttons on the same line
  if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
  {
    mCurrentGizmoOperation = ImGuizmo::ROTATE;
  }

  //extract the components from our matrix
  float matrixTranslation[3], matrixRotation[3], matrixScale[3];
  ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
  ImGui::DragFloat3("Translate", matrixTranslation); //edit the extracted translation
  ImGui::DragFloat3("Rotate",matrixRotation,1.0f,0.0f,360.0f);//edit the extracted rotation
  //Use the edited components to rebuild our matrix
  ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

  //Buttons to select current mode
  if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
  {
    mCurrentGizmoMode = ImGuizmo::LOCAL;
  }
  ImGui::SameLine();//buttons on the same line
  if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
  {
    mCurrentGizmoMode = ImGuizmo::WORLD;
  }

  //If our mouse is being used to rotate the scene we disable the gizmo
  if(keystates[SDL_SCANCODE_LCTRL] || keystates[SDL_SCANCODE_RCTRL])
  {
    ImGuizmo::Enable(false);
  }
  else
  {
    ImGuizmo::Enable(true);
  }
  //Get current input
  m_io = ImGui::GetIO();
  ImGuizmo::SetRect(0, 0, m_io.DisplaySize.x, m_io.DisplaySize.y);
  //Transform the gizmo and matrix
  ImGuizmo::Manipulate(mv, pm, mCurrentGizmoOperation, mCurrentGizmoMode, matrix);
  //Convert the float array back to our 4x4 Matrix
  m_emit.m_transform = glm::make_mat4(matrix);
  //Reset the matrix to identity
  if(ImGui::Button("Reset transform"))
  {
    m_emit.m_transform = glm::mat4(1.0f);
  }
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::displaySystemGui()
{
  //System GUI for performance stats and options
  ImGui::Checkbox("Pause",&m_pause);
  ImGui::SameLine();
  ImGui::Checkbox("Grid",&m_grid);
  ImGui::SameLine();
  ImGui::Checkbox("Reduce memory",&m_emit.m_reduceMemory);
  if(ImGui::Button("Clear System")) {m_emit.m_clear = true;}
  if(ImGui::Button("Screenshot")) {m_snap = true;}
  ImGui::Text("Particle count: %zu / %zu",m_emit.particleCount(),m_emit.maxParticles());
  ImGui::Text("Frame time: %.3f ms/frame ", 1000.0f / ImGui::GetIO().Framerate);
  ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
  ImGui::Text("Particle memory usage: %zu", m_emit.vectorSize());
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::displayFlameGui()
{
  //Flame GUI
  ImGui::Text("Fire colour");
  ColorSelector("Fire colour", m_emit.m_flCol);
  ImGui::SliderFloat("Size",&m_emit.m_flSize,2.0f,200.0f);
  ImGui::SliderFloat("Speed",&m_emit.m_flSpeed,0.1f,2.0f);
  ImGui::SliderFloat("Base spread",&m_emit.m_flSpread,0.1f,10.0f);
  ImGui::SliderAngle("Steepness",&m_emit.m_flSteepness,0.0f,90.0f);
  ImGui::SliderInt("Life",&m_emit.m_flLife,0,100);
  ImGui::SliderInt("Density",&m_emit.m_flDensity,0,20);
  ImGui::Checkbox("Spawn children",&m_emit.m_flChildren);
  //when selected flame will be emitted
  ImGui::Checkbox("Ignite flame",&m_emit.m_flame);
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::displayExplosionGui()
{
  //Explosion GUI
  ImGui::Text("Explosion colour");
  ColorSelector("Explosion colour", m_emit.m_expCol);
  ImGui::SliderFloat("Size",&m_emit.m_expSize,1.0f,200.0f);
  ImGui::SliderFloat("Speed",&m_emit.m_expSpeed,0.1f,2.0f);
  ImGui::SliderAngle("Steepness",&m_emit.m_expIncline,0.0f,90.0f);
  ImGui::SliderInt("Life",&m_emit.m_expLife,0,100);
  ImGui::SliderInt("Density",&m_emit.m_expDensity,0,40);
  //number of frames to explode for
  static int numExplosions = 5;
  ImGui::SliderInt("Chain",&numExplosions,0,50);
  if(ImGui::Button("Detonate Explosion"))
  {
    m_emit.m_explosions = numExplosions;
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
void Scene::handleInput()
{
  //take a screenshot
  if(m_snap)
  {
    takeScreencap();
    m_snap = false;
  }
  makeCurrent();
  //process user input
  while(SDL_PollEvent(&m_inputEvent))
  {
    //if the mouse isn't over the GUI process movement, and key press (need this for typing)
    if(!m_io.WantCaptureMouse && !ImGuizmo::IsUsing())
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
        case SDLK_w : m_emit.m_explosions = 6; break;
        case SDLK_e : m_emit.m_flame = !m_emit.m_flame; break;
        case SDLK_c : m_emit.m_clear = true; break;
        case SDLK_f : resetPos(); break;
        case SDLK_g : m_grid = !m_grid; break;
        case SDLK_s : m_snap = true; break;
        case SDLK_LEFT : m_emit.setPos(m_emit.pos() + glm::vec3(-0.5f,0.0f,0.0f)); break;
        case SDLK_RIGHT : m_emit.setPos(m_emit.pos() + glm::vec3(0.5f,0.0f,0.0f)); break;
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
  //calculate distance moved by mozuse
  glm::vec2 diff = ((glm::vec2)(newPos - m_mousePos)) * strength;
  diff.x = -diff.x;
  if(keystates[SDL_SCANCODE_LCTRL] || keystates[SDL_SCANCODE_RCTRL])
  {
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
  }
  //bounds checking for rotation to keep it between 0 and 360 degrees
  if(m_rotation.x >= 360.0f) {m_rotation.x -= 360.0f;}
  else if(m_rotation.x < 0.0f) {m_rotation.x += 360.0f;}
  if(m_rotation.y >= 360.0f) {m_rotation.y -= 360.0f;}
  else if(m_rotation.y < 0.0f) {m_rotation.y += 360.0f;}
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
  glMultMatrixf(glm::value_ptr(_matrix));
  //change back to modelview mode
  glMatrixMode(GL_MODELVIEW);
}
//-------------------------------------------------------------------------------------------------------------------------
void Scene::loadModelView(glm::mat4 const&_matrix) const
{
  //make sure we are in modelview mode
  glMatrixMode(GL_MODELVIEW);
  //load the identity matrix before applying our own
  glLoadIdentity();
  //value_ptr returns a const float* to the matrix
  glMultMatrixf(glm::value_ptr(_matrix));
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
  //use Mutex's to ensure we don't interfere with updating
  SDL_LockMutex(m_mutex);
  while(!m_canDraw)
  {
    SDL_CondWait(m_canDraw, m_mutex);
  }
  //Draw the particles
  m_emit.draw();
  //allow updating to resume
  SDL_UnlockMutex(m_mutex);
  SDL_CondSignal(m_canUpdate);
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
  //_num is the dimensions or number of rows and columns the grid contains, step is the spacing between them
  for(int i = -_num*_step; i <= _num*_step; i+=_step)
  {
    glVertex3f(_num*_step,0.0f,i);
    glVertex3f(-_num*_step,0.0f,i);
    glVertex3f(i,0.0f,_num*_step);
    glVertex3f(i,0.0f,-_num*_step);
  }
  glEnd();
  //enable textures again
  glEnable(GL_TEXTURE_2D);
}
//-------------------------------------------------------------------------------------------------------------------------

