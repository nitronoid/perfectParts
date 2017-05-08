
#include "pngutils.h"

#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <iostream>


/**
 * @brief png_texture_size opens up a png and reads just the width and height from the header
 * Note that to get a std::string to a const char * you need to use the .c_str() accessor.
 * @param filename const char* with name of texture filename
 * @param width Passed by reference, set by this function if successful
 * @param height Passed by reference, set by this function if successful
 * @return 0 on failure, or the size of the memory structure you need to malloc to load the image on success (raw)
 */
GLuint png_texture_size(const char *file_name, GLuint &width, GLuint &height) {
  png_byte header[8];

  FILE *fp = fopen(file_name, "rb");
  if (fp == 0)
  {
    perror(file_name);
    return 0;
  }

  // read the header
  fread(header, 1, 8, fp);

  if (png_sig_cmp(header, 0, 8))
  {
    fprintf(stderr, "error: %s is not a PNG.\n", file_name);
    fclose(fp);
    return 0;
  }

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
  {
    fprintf(stderr, "error: png_create_read_struct returned 0.\n");
    fclose(fp);
    return 0;
  }

  // create png info struct
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    fprintf(stderr, "error: png_create_info_struct returned 0.\n");
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    fclose(fp);
    return 0;
  }

  // create png info struct
  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info)
  {
    fprintf(stderr, "error: png_create_info_struct returned 0.\n");
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
    fclose(fp);
    return 0;
  }

  // the code in this if statement gets called if libpng encounters an error
  if (setjmp(png_jmpbuf(png_ptr))) {
    fprintf(stderr, "error from libpng\n");
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(fp);
    return 0;
  }

  // init png reading
  png_init_io(png_ptr, fp);

  // let libpng know you already read the first 8 bytes
  png_set_sig_bytes(png_ptr, 8);

  // read all the info up to the image data
  png_read_info(png_ptr, info_ptr);

  // variables to pass to get info
  int bit_depth, color_type;
  png_uint_32 temp_width, temp_height;

  // get info about png
  png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
               NULL, NULL, NULL);

  // Write out the result to the input parameters
  width = temp_width;
  height = temp_height;

  // Update the png info struct.
  png_read_update_info(png_ptr, info_ptr);

  // Row size in bytes.
  GLuint rowbytes = png_get_rowbytes(png_ptr, info_ptr);

  // glTexImage2d requires rows to be 4-byte aligned
  rowbytes += 3 - ((rowbytes-1) % 4);

  // Close up opened things
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
  fclose(fp);

  // Return the size of the memory structure you need to create
  return rowbytes * temp_height * sizeof(png_byte)+15;
}

/**
 * @brief png_texture_load Actually loads the specified filename into the data
 * Note that to get a std::string to a const char * you need to use the .c_str() accessor.
 * Pilfered from:
 * http://stackoverflow.com/questions/11296644/loading-png-textures-to-opengl-with-libpng-only
 * Modified to return a fresh pointer to the image data - be sure to delete the pointer when it is no longer required!
 * Note there is lots of repetition with the above function - this was necessary to extract the texId stuff.
 * @param file_name The filename from which to load the data
 * @param data The chunk of data to fill with this image
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
GLuint png_texture_load(const char * file_name, GLubyte *&data)
{
  png_byte header[8];

  FILE *fp = fopen(file_name, "rb");
  if (fp == 0)
  {
    perror(file_name);
    return EXIT_FAILURE;
  }

  // read the header
  fread(header, 1, 8, fp);

  if (png_sig_cmp(header, 0, 8))
  {
    fprintf(stderr, "error: %s is not a PNG.\n", file_name);
    fclose(fp);
    return EXIT_FAILURE;
  }

  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr)
  {
    fprintf(stderr, "error: png_create_read_struct returned 0.\n");
    fclose(fp);
    return EXIT_FAILURE;
  }

  // create png info struct
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    fprintf(stderr, "error: png_create_info_struct returned 0.\n");
    png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
    fclose(fp);
    return EXIT_FAILURE;
  }

  // create png info struct
  png_infop end_info = png_create_info_struct(png_ptr);
  if (!end_info)
  {
    fprintf(stderr, "error: png_create_info_struct returned 0.\n");
    png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp) NULL);
    fclose(fp);
    return EXIT_FAILURE;
  }

  // the code in this if statement gets called if libpng encounters an error
  if (setjmp(png_jmpbuf(png_ptr))) {
    fprintf(stderr, "error from libpng\n");
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(fp);
    return EXIT_FAILURE;
  }

  // init png reading
  png_init_io(png_ptr, fp);

  // let libpng know you already read the first 8 bytes
  png_set_sig_bytes(png_ptr, 8);

  // read all the info up to the image data
  png_read_info(png_ptr, info_ptr);

  // variables to pass to get info
  int bit_depth, color_type;
  png_uint_32 temp_width, temp_height;

  // get info about png
  png_get_IHDR(png_ptr, info_ptr, &temp_width, &temp_height, &bit_depth, &color_type,
               NULL, NULL, NULL);

  // Update the png info struct.
  png_read_update_info(png_ptr, info_ptr);

  // Row size in bytes.
  int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

  // glTexImage2d requires rows to be 4-byte aligned
  rowbytes += 3 - ((rowbytes-1) % 4);

  // Use our pre-allocated memory
  png_byte * image_data;
  if (data != NULL) {
    image_data = (png_byte *) data;
  } else {
    fprintf(stderr, "error: no memory passed to this function!");
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    fclose(fp);
    return EXIT_FAILURE;
  }
  // This is how it was originally done, for reference.
  //image_data = malloc(rowbytes * temp_height * sizeof(png_byte)+15);

  // row_pointers is for pointing to image_data for reading the png with libpng
  png_bytep * row_pointers = (png_bytep*) malloc(temp_height * sizeof(png_bytep));
  if (row_pointers == NULL)
  {
    fprintf(stderr, "error: could not allocate memory for PNG row pointers\n");
    png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
    free(image_data);
    fclose(fp);
    return EXIT_FAILURE;
  }

  // set the individual row_pointers to point at the correct offsets of image_data
  unsigned int i;
  for (i = 0; i < temp_height; i++)
  {
    row_pointers[temp_height - 1 - i] = image_data + i * rowbytes;
  }

  // read the png into image_data through row_pointers
  png_read_image(png_ptr, row_pointers);

  // clean up
  png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
  free(row_pointers);
  fclose(fp);
  return EXIT_SUCCESS;
}

bool save_png_libpng(const char *file_name, uint8_t *pixels, int w, int h)
{
  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
  if (!png)
    return false;

  png_infop info = png_create_info_struct(png);
  if (!info) {
    png_destroy_write_struct(&png, &info);
    return false;
  }

  FILE *fp = fopen(file_name, "wb");
  if (!fp) {
    png_destroy_write_struct(&png, &info);
    return false;
  }

  png_init_io(png, fp);
  png_set_IHDR(png, info, w, h, 8 /* depth */, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
  png_colorp palette = (png_colorp)png_malloc(png, PNG_MAX_PALETTE_LENGTH * sizeof(png_color));
  if (!palette) {
    fclose(fp);
    png_destroy_write_struct(&png, &info);
    return false;
  }
  png_set_PLTE(png, info, palette, PNG_MAX_PALETTE_LENGTH);
  png_write_info(png, info);
  png_set_packing(png);

  png_bytepp rows = (png_bytepp)png_malloc(png, h * sizeof(png_bytep));
  for (int i = 0; i < h; ++i)
    rows[i] = (png_bytep)(pixels + (h - i - 1) * w * 3);

  png_write_image(png, rows);
  png_write_end(png, info);
  png_free(png, palette);
  png_destroy_write_struct(&png, &info);

  fclose(fp);
  delete[] rows;
  return true;
}

























