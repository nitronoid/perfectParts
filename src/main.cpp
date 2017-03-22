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
  Emitter em(glm::vec3(0.0f,0.0f,0.0f),10000);
  while(!win.m_quit)
  {
    win.pollEvent(em);
    if(!win.m_pause && !win.m_quit)
    {
      em.update();
      if(!win.m_trails)
      {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glAccum(GL_RETURN, 1.0);
        //glClear(GL_ACCUM_BUFFER_BIT);
      }
      win.draw(em);
      win.swapWindow();
      //glAccum(GL_ACCUM, 0.75f);
    }
  }
  return EXIT_SUCCESS;
}
