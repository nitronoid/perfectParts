# perfectParts, real time interactive particle simulation #
<p align="center">
<img src="http://i.imgur.com/avbcTIk.png" width="500">
</p>
perfectParts is an implementation of the classic Reeves particle system using SDL2 and OpenGL 2.
The system uses a dynamic array of polymorphic particle pointers to allow for different techniques 
to be applied when drawing and updating the particles. The array uses an object pool aproach to 
avoid constant memory allocations and resizing, however options are available to limit memory usage.

## Requirements ##
- Qt
- SDL2
- OpenGL 2
- libpng (Mac users may need to fix libpng library conflicts)

## Sources ##
The program uses ImGui for drawing GUI elements, and ImGuizmo for drawing the transformation gizmo.
- dear ImGui --- https://github.com/ocornut/imgui
- ImGuizmo --- https://github.com/CedricGuillemet/ImGuizmo

## Controls ##
### Keyboard and Mouse controls ###
The system has some basic controls implemented with keyboard shortcuts

- q --- Launch firework
- w --- Detonate explosion
- e --- Ignite flame
- c --- Clears the system of all particles
- f --- Resets the camera position and rotation
- g --- Toggles the 3D grid
- s --- Takes a screenshot
- ESC --- Quits the program
- SPACE --- Pauses time

Clicking and dragging the mouse can be used to transform the 3D scene, when holding down CTRL on the keyboard.
- CTRL + LEFT Mouse --- Rotate scene
- CTRL + RIGHT Mouse --- Zoom scene
- CTRL + MIDDLE Mouse --- Pan scene
- CTRL + ALT + LEFT Mouse --- Pan scene (For trackpad users)

### GUI controls ###

The gui contains a seperate tab for every particle object available, as well as view and system controls.
Every control has a label which explains its use.

#### System GUI Tab ####
<p align="center">
<img src="http://i.imgur.com/GLfH7ny.png" width="250">
</p>
The system tab contains options to pause time, select texture, take a screenshot, reduce memory and clear the system.
If you take a screenshot, it will be saved in .png format in the local screenshot folder (program must be run from the same directory as this folder).
The reduce memory option will use a garbage collector to remove unused particles from the system, based on the current number and performance. 
This will allow for the system to be clean when it will least affect performance. 
The advanced section will allow you to change the particle system limit, which I have defaulted to 100,000.

#### View GUI Tab ####
<p align="center">
<img src="http://i.imgur.com/E3d5jDZ.png" width="500">
</p>
The view tab displays the scene rotation and translation, and allows for manual editing.
It also displays the current rotation and translation of the emitter, note that this only affects the matrix and not the actual emitter position, gravity and other forces will be relative to the transformations.
A gizmo is displayed when this tab is selected for interactive editing of the transformation, which can be done in local or world space.

## References ##
- William T. Reeves - [Particle Systems --- A Technique for Modeling a Class of Fuzzy Objects](https://www.lri.fr/~mbl/ENS/IG2/devoir2/files/docs/fuzzyParticles.pdf) [1983]
- Wensheng Dong, Xinyan Zhang and Caijun Zhang - [Smoke Simulation Based on Particle System in Virtual Environments](http://ieeexplore.ieee.org/document/5694138/) [2010]
- Teng-See Loke, Daniel Tan, Hock-Soon Seah, and Meng-Hwa Er - [Rendering fireworks displays](http://ieeexplore.ieee.org/document/135912/) [1992]