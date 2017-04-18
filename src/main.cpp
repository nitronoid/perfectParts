#if defined (__linux__) || defined (WIN32)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif
#include <cstdlib>
#include "Window.h"


int main()
{
  Window win("Particle Simulation",0,0,720,576);
  win.makeCurrent();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  while(!win.m_quit)
  {
    win.pollEvent();
    win.draw();
    win.swapWindow();
  }
  return EXIT_SUCCESS;
}
