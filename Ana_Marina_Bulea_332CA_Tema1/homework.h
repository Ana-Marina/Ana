#ifndef HOMEWORK_H
#define HOMEWORK_H

#include <stdlib.h>

typedef struct{
	int width;
	int height;
	int maxvalue;
	int colors; //numar de culori intr-un pixel
	unsigned char** pixels;
} image;

#endif
