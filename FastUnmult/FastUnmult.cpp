#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define PNG_CHANNELS_COUNT 4
#include "omp.h"
//#include <time.h>


using namespace std;

int unmult(const char* filename,const char* output);


int main(int argc, char* argv[])
{
	if (argc == 1) {
		cout << "Unmult App - Frezledz 2024" << endl << endl;
		cout << "This application is an independent character user interface application that can generate Transparent image sequences just like After effects library called Unmult released by Maxon Cooporation." << endl;
		cout << "In order to convert video files, you will additionally need to install FFmpeg and set it in environment variables." << endl << endl;
		cout << "Arguments you need to specify are : " << endl;
		cout << "Video or image (1 for Video, 0 for image)"<<endl;
		cout << "input file name" << endl;
		cout << "output file name" << endl << endl;
		cout << "and here is a example :" << endl;
		cout << "UnmultApp.exe 0 image.jpg transparent.png" << endl << endl;
		cout << "Go to https://github.com/xxXFreezerXxx for more information." << endl;
		return 0;

	}
	if (argc == 4) {
		const char* filename = argv[2];
		if (argv[1] == "0") {
			const char* output = argv[3];
			int res = unmult(filename, output);
			return res;
		}
		if (argv[1] == "1") {
			cout << "Sorry, video conversion feature is still in development." << endl;
			return 0;
			
		}
		cout << "Invalid value was specified to first argument. Specify 1 for video, 0 for image."<< endl;
		return 1;

	}
	cout << "You have specified too many/few arguments. Run this application without any arguments to get help"<<endl;
	return 1;
}

bool cmp(int x, int y) {
	return (x < y);
}//Max用関数
//ffmpeg
int unmult(const char* filename,const char* output) {
	clock_t begin = clock();
	int width, height, channels;
	unsigned char* img = stbi_load(filename, &width, &height, &channels, 3);
	if (img == NULL) {
		cout << "Error loading a image, file may not exist." << endl;
		return 1;
	}
	//add unmult process
	unsigned char* dst_img;
	dst_img = (unsigned char*)malloc(sizeof(unsigned char)*width*height* PNG_CHANNELS_COUNT);
	if (dst_img == NULL) {
		cout << "Error allocating memory." << endl;
		return 1;
	}
	unsigned char* mem = img;
	cout << "Converting " << filename << endl;
	#pragma omp parallel private(img)
	{
		//split chunks to each processor manually to avoid any errors
		int thread_id = omp_get_thread_num();
		int thread_count = omp_get_num_threads();
		int chunk_size = height / thread_count;
		int start = thread_id * chunk_size;
		int end = ((thread_id + 1) == thread_count) ? height : (thread_id + 1) * chunk_size;
		for (int yy = start; yy < end; yy++) {
			for (int xx = 0; xx < width; xx++) {
				float factor = 255.0 / max({ img[0], img[1], img[2] }, cmp);
				dst_img[PNG_CHANNELS_COUNT * (yy * width + xx) + 0] = img[0] * factor;
				dst_img[PNG_CHANNELS_COUNT * (yy * width + xx) + 1] = img[1] * factor;
				dst_img[PNG_CHANNELS_COUNT * (yy * width + xx) + 2] = img[2] * factor;
				dst_img[PNG_CHANNELS_COUNT * (yy * width + xx) + 3] = 255.0 / factor;
				img = mem + channels * (yy * width + xx + 1);
			}
		}

	}
	/*clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	begin = clock();
	cout <<"Conversion done in "<< time_spent<<" seconds" << endl;*/
	cout << "Saving file..." << endl;
	stbi_write_png(output, width, height, PNG_CHANNELS_COUNT, dst_img, width * PNG_CHANNELS_COUNT);
	cout << "File saved as " << output << endl;
	/*end = clock();
	time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	cout << "File saved as " << output << ", time took: " << time_spent << " seconds" <<endl;*/
	free(dst_img);
	return 0;

}
