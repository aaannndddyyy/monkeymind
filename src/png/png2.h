#ifndef PNG2_H
#define PNG2_H

#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include "pnglite.h"

static int write_png_file(char* filename, int width, int height, unsigned char *buffer)
{
  png_t png;
  FILE * fp = fopen(filename, "wb");
  if (fp == NULL) {
    fprintf(stderr, "Could not open file %s for writing\n", filename);
    return 1;
  }
  fclose(fp);

  png_init(0,0);
  png_open_file_write(&png, filename);
  png_set_data(&png, width, height, 8, PNG_TRUECOLOR, buffer);
  png_close_file(&png);

  return 0;
}

#endif
