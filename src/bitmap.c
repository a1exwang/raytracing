//
// Created by alexwang on 4/18/16.
//

#include <png.h>
#include <stdlib.h>
#include <stdarg.h>
#include <memory.h>
#include "bitmap.h"
#include "camera.h"

static void abort_(const char * s, ...)  {
  va_list args;
  va_start(args, s);
  vfprintf(stderr, s, args);
  fprintf(stderr, "\n");
  va_end(args);
  abort();
}

static void write_png_file(Bitmap *bitmap, const char* file_name) {
  png_structp png_ptr;
  png_infop info_ptr;
  unsigned int width, height;
  png_byte color_type;
  png_byte bit_depth = 8;
  png_bytep * row_pointers;

  width = (unsigned int) bitmap->width;
  height = (unsigned int) bitmap->height;

  /* create file */
  FILE *fp = fopen(file_name, "wb");
  if (!fp)
    abort_("[write_png_file] File %s could not be opened for writing", file_name);

  /* initialize stuff */
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
    abort_("[write_png_file] png_create_write_struct failed");

  info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
    abort_("[write_png_file] png_create_info_struct failed");

  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during init_io");

  png_init_io(png_ptr, fp);


  /* write header */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during writing header");

  png_set_IHDR(png_ptr, info_ptr, width, height,
               bit_depth, PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE,
               PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

  png_write_info(png_ptr, info_ptr);


  /* write bytes */

  if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB)
    abort_("[process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA "
                   "(lacks the alpha channel)");

  if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA)
    abort_("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)",
           PNG_COLOR_TYPE_RGBA, png_get_color_type(png_ptr, info_ptr));

  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
  }

  for (int y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (int x = 0; x < width; x++) {
      png_byte* ptr = &(row[x * 4]);
      double r, g, b;
      color_get_rgb(&bitmap->buffer[x][y], &r, &g, &b);
      ptr[0] = (png_byte)(r * 0xFF);
      ptr[1] = (png_byte)(g * 0xFF);
      ptr[2] = (png_byte)(b * 0xFF);
      ptr[3] = 0xFF;
     // printf("(%d, %d): %f, %f, %f\n", x, y, r, g, b);
    }
  }

  png_write_image(png_ptr, row_pointers);

  /* end write */
  if (setjmp(png_jmpbuf(png_ptr)))
    abort_("[write_png_file] Error during end of write");

  png_write_end(png_ptr, NULL);

  /* cleanup heap allocation */
  for (int y = 0; y < height; y++)
    free(row_pointers[y]);
  free(row_pointers);

  fclose(fp);
}

void bitmap_init(Bitmap *bitmap, int w, int h) {
  bitmap->width = w;
  bitmap->height = h;

  bitmap->buffer = (Vector**) malloc(sizeof(Vector*) * w);
  for (int i = 0; i < w; ++i) {
    bitmap->buffer[i] = (Vector *) malloc((sizeof(Vector) * h));
    memset(bitmap->buffer[i], 0, sizeof(Vector) * h);
  }
}

void bitmap_to_png(Bitmap *bitmap, const char *path) {
 write_png_file(bitmap, path);
}










