#include "homework.h"
#include <stdio.h>
#include <memory.h>
#include <omp.h>

int num_threads;
int resize_factor;

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

//citeste imaginea din fisier
void readInput(const char * fileName, image *img) {
	char magic[100];
	int i;
	FILE* imgfile = fopen(fileName, "rb");

	omp_set_num_threads(num_threads);

	fscanf(imgfile, "%s\n%d %d\n%d\n", magic, &img->width, &img->height,
		&img->maxvalue);
	if(strcmp(magic, "P6") == 0)
		img->colors = 3;
	else img->colors = 1;

	create_image(img);

	//citeste pixelii linie cu linie
	for(i  = 0; i < img->height; i++)
		fread(img->pixels[i], img->width * img->colors * sizeof(unsigned char),
			1, imgfile);

	fclose(imgfile);
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

	for(i = 0; i < img->height; i++)
		fwrite(img->pixels[i], img->width * img->colors * sizeof(unsigned char),
			1, imgfile);

	fclose(imgfile);
	delete_image(img);
}

//calculeaza suma pixelilor din zona (ii, ji) - (ii+resize_factor, ji+resize_factor)
//pentru resize_factor = 3 se foloseste kernelul gausian
void pixel_sum(image *in, int ii, int ji, int *sum){
	int i, j, c;
	//kernel gausian
	const int kernel[3][3] = {{1, 2, 1},
							  {2, 4, 2},
							  {1, 2, 1}};
	unsigned char* pixel = NULL;

	for(i = 0; i < resize_factor; i++){
		for(j = 0; j < resize_factor; j++){
			pixel = get_pixel(in, ii+i, ji+j);

			//calculeaza suma pentru fiecare canal
			if(resize_factor % 2 == 0){
				for(c = 0; c < in->colors; c++)
					sum[c] += pixel[c];
			}
			else if(resize_factor == 3){
				for(c = 0; c < in->colors; c++)
					sum[c] += pixel[c]*kernel[i][j];
			}
		}
	}
}

//redimensioneaza imaginea in
void resize(image *in, image * out) {
	int i, j, c;
	int sum[3];
	unsigned char* pixel = NULL;

	out->width = in->width / resize_factor;
	out->height = in->height / resize_factor;
	out->maxvalue = in->maxvalue;
	out->colors = in->colors;

	create_image(out);

	#pragma omp parallel for collapse(2) \
		private(i, j, c, sum, pixel)
	for(i = 0; i < out->height; i++){
		for(j = 0; j < out->width; j++){
			//initializeaza suma pixelilor cu 0
			memset(sum, 0, sizeof(sum));

			//calculeaza suma pixelilor
			pixel_sum(in, i * resize_factor, j * resize_factor, sum);

			pixel = get_pixel(out, i, j);
			//calculeaza media aritmetica pentru fiecare culoare in parte
			for(c = 0; c < out->colors; c++)
				if(resize_factor % 2 == 0)
					pixel[c] = sum[c] / (resize_factor * resize_factor);
				else
					pixel[c] = sum[c] / 16;
		}
	}

	delete_image(in);
}
