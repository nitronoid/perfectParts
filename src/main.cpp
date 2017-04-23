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
  while(!win.m_quit)
  {
    win.tick();
    win.draw();
  }
  return EXIT_SUCCESS;
}

