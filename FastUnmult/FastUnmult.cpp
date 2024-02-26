#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define PNG_CHANNELS_COUNT 4
#include "omp.h"
#include <fstream>
#include <string>


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
		cout << "UnmultApp.exe 0 image.jpg transparent" << endl << endl;
		cout << "UnmultApp.exe 1 video.mp4 trans" << endl << endl;
		cout << "Go to https://github.com/Frezledz for more idnformation." << endl;
		return 0;

	}
	if (argc == 4) {
		const char* filename = argv[2];
		const char* output = argv[3];
		if (((string)"0" == (string)argv[1])) {
			int res = unmult(filename, output);
			return res;
		}
		if (((string)"1" == (string)argv[1])) {
			string ffmpeg_command = "ffmpeg -i \"" + (string)filename+"\" \"tmp%01d.png\"";
			system(ffmpeg_command.c_str());
			ifstream checker_file;
			checker_file.open("tmp1.png");
			if (!checker_file) {
				cout << "FFmpeg conversion failed. Make sure you have installed ffmpeg to your computer." << endl;
				return 1;
			}
			int i = 0;
			while (true)
			{
				i++;
				ifstream ffmpeg_file;
				char ffmpeg_file_name[256];
				sprintf_s(ffmpeg_file_name,"tmp%d.png",i);
				ffmpeg_file.open(ffmpeg_file_name);
				if (ffmpeg_file) {
					string output_file_name =(string)output + to_string(i);
					int res = unmult(ffmpeg_file_name, output_file_name.c_str());
				}
				else {
					cout <<endl<< "Conversion done, now deleting tmp file..." << endl;
					break;
				}
			}
			for (int ii = 1; ii < i; ii++) {
				char ffmpeg_file_name[256];
				sprintf_s(ffmpeg_file_name, "tmp%d.png", ii);
				remove(ffmpeg_file_name);
				
			}
			cout << "Done." << endl;
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
		cout << "Error loading an image, file may not exist." << endl;
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
	cout<<"\r" << "Applying effect to " << filename << flush;
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
	string output_file_name = (string)output + ".png";
	stbi_write_png(output_file_name.c_str(), width, height, PNG_CHANNELS_COUNT, dst_img, width * PNG_CHANNELS_COUNT);
//	cout << "File saved as " << output << endl;
	free(dst_img);
	return 0;

}
