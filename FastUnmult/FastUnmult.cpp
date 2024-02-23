#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define PNG_CHANNELS_COUNT 4
#include "omp.h"
using namespace std;

int unmult(const char* filename,const char* output);
int main()
{
	const char* filename = "aa.jpg";
	const char* output = "aa.png";
	int res = unmult(filename,output);
	return 0;
}

bool cmp(int x, int y) {
	return (x < y);
}//Max用関数
//OpenMP、ffmpeg
int unmult(const char* filename,const char* output) {
	int width, height, channels;
	unsigned char* img = stbi_load(filename, &width, &height, &channels, 3);
	if (img == NULL) {
		cout << "Error loading a image" << endl;
		return 1;
	}
	//add unmult process
	unsigned char* dst_img;
	dst_img = (unsigned char*)malloc(sizeof(unsigned char)*width*height* PNG_CHANNELS_COUNT);
	if (dst_img == NULL) {
		cout << "Error allocating memory" << endl;
		return 1;
	}
	cout << omp_get_max_threads() << endl;
	auto mem = img;
	for (int yy = 0; yy < height; yy++) {
		for (int xx = 0; xx < width; xx++) {
			float factor = 255.0 / max({ img[0], img[1], img[2] },cmp);
			dst_img[PNG_CHANNELS_COUNT * (yy * width + xx) + 0] = img[0] * factor;
			dst_img[PNG_CHANNELS_COUNT * (yy * width + xx) + 1] = img[1] * factor;
			dst_img[PNG_CHANNELS_COUNT * (yy * width + xx) + 2] = img[2] * factor;
			dst_img[PNG_CHANNELS_COUNT * (yy * width + xx) + 3] = 255.0 / factor;
			//img = mem + channels * (yy * width + xx);
			img+=channels;
		}
	}
	stbi_write_png(output, width, height, PNG_CHANNELS_COUNT, dst_img, width * PNG_CHANNELS_COUNT);
	free(dst_img);
	return 0;

}
