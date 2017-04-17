#ifndef PNGUTILS_H
#define PNGUTILS_H

#include <GL/gl.h>

/** @file */

/// Determine the width, height and raw data size of the png with the given filename
GLuint png_texture_size(const char *file_name, GLuint &width, GLuint &height);

/// Load the image into the specified data
GLuint png_texture_load(const char * file_name, GLubyte *&data);

#endif // PNGUTILS_H
