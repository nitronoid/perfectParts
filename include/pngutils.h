#ifndef PNGUTILS_H
#define PNGUTILS_H

#if defined (__linux__) || defined (WIN32)
#include <GL/gl.h>
#include <GL/glu.h>
#else
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#endif

/// @note modified using pnglib code to allow saving a png
/// [Accessed 2017]. Available from: "http://www.labbookpages.co.uk/software/imgProc/libPNG.html"
/** @file */

/// Determine the width, height and raw data size of the png with the given filename
GLuint png_texture_size(const char *file_name, GLuint &width, GLuint &height);

/// Load the image into the specified data
GLuint png_texture_load(const char * file_name, GLubyte *&data);

/// Save frame buffer as .png image file
bool save_png_libpng(const char *file_name, uint8_t *pixels, int w, int h);

#endif // PNGUTILS_H
