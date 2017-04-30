#include <cstdlib>
#include "Scene.h"

int main()
{
  Scene world("Particle Simulation",0,0,720,576);
  world.makeCurrent();
  while(!world.m_quit)
  {
    world.tick();
    world.draw();
  }
  return EXIT_SUCCESS;
}

