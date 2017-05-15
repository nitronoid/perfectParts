/// @brief Modified from ImGui API for rendering imediate openGL GUI's
/// @author Omar Cornut
/// @date [September, 2016]
/// [Accessed 2017]. Available from: "https://github.com/ocornut/imgui/blob/master/examples/sdl_opengl2_example/imgui_impl_sdl.h"

// ImGui SDL2 binding with OpenGL2
// In this binding, ImTextureID is used to store an OpenGL 'GLuint' texture identifier. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you use this binding you'll need to call 4 functions: ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), ImGui::Render() and ImGui_ImplXXXX_Shutdown().
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// SOURCE = https://github.com/ocornut/imgui

#include <SDL.h>
#include "imgui.h"

#if defined (__linux__) || defined (WIN32)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

IMGUI_API bool        ImGui_ImplSdl_Init(SDL_Window* window);
IMGUI_API void        ImGui_ImplSdl_Shutdown();
IMGUI_API void        ImGui_ImplSdl_NewFrame(SDL_Window* window);
IMGUI_API bool        ImGui_ImplSdl_ProcessEvent(SDL_Event* event);

// Use if you want to reset your rendering device without losing ImGui state.
IMGUI_API void        ImGui_ImplSdl_InvalidateDeviceObjects();
IMGUI_API bool        ImGui_ImplSdl_CreateDeviceObjects();
