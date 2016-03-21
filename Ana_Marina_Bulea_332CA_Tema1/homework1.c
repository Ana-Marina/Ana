#include "homework1.h"
#include <math.h>
#include <stdio.h>

int num_threads;
int resolution;

//returneaza un pointer la pixelul de la linia i si coloana j
unsigned char* get_pixel(image *im, int i, int j){
	return &(im->pixels[i][j * im->colors]);
}

//aloca imaginea
void create_image(image *im){
	int i;
	im->pixels = malloc(im->height * sizeof(unsigned char*));

	for(i = 0; i < im->height; i++)
		im->pixels[i] = malloc(im->width * im->colors * sizeof(unsigned char));
}

//elibereaza imaginea
void delete_image(image *im){
	int i;

	for(i = 0; i < im->height; i++)
		free(im->pixels[i]);

	free(im->pixels);
}

//initializeaza imaginea
void initialize(image *im) {
	im->height = resolution;
	im->width = resolution;
	im->colors = 1;
	im->maxvalue = 255;
	create_image(im);
	
	omp_set_num_threads(num_threads);
}

//calculeaza distanta intre dreapta si un punct
double calculateDistance(int x, int y) {
	return abs(-1 * x + 2 * y + 0)/sqrt(1 + 4);
}

//deseneaza linia in imagine
void render(image *im) {
	int i, j;
	unsigned char* pixel;
	
	//factor de scalare
	double a = 100.0/resolution;
	//valoarea adaugata pentru a obtine jumatatea unui pixel
	double b = a/2;

	#pragma omp parallel for collapse(2) \
		private(i, j, pixel)
	for(i = 0; i < im->height; i++){
		for(j = 0; j < im->width; j++){
			pixel = get_pixel(im, i, j);

			if(calculateDistance(j * a + b, i * a + b) < 3)
				pixel[0] = 0; //negru
			else
				pixel[0] = 255; //alb
		}
	}
}

//salveaza imaginea in fisier
void writeData(const char * fileName, image *img) {
	FILE* imgfile = fopen(fileName, "wb");
	int i;

	if(img->colors == 3)
		fprintf(imgfile, "%s\n%d %d\n%d\n", "P6", img->width, img->height,
			img->maxvalue);
	else
		fprintf(imgfile, "%s\n%d %d\n%d\n", "P5", img->width, img->height,
			img->maxvalue);

	//scrie imaginea rasturanata
	for(i = img->height - 1; i >= 0; i--)
		fwrite(img->pixels[i], img->width * img->colors * sizeof(unsigned char),
			1, imgfile);

	fclose(imgfile);
	delete_image(img);
}
