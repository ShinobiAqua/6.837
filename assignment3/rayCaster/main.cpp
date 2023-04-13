// ========================================================
// ========================================================
// Some sample code you might like to use for parsing 
// command line arguments 
#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"scene_parser.h"
#include"camera.h"
#include"group.h"
#include"image.h"
#include"material.h"
#include"render.h"


using namespace std;
char* input_file = NULL;
int width = 100;
int height = 100;
char* output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char* depth_file = NULL;
char* normal_file = NULL;
bool shade_back = false;
bool gui = false;


bool gouraud = false;
int fhi_step, theta_step;


SceneParser* sp;
Render *rd;
void glRendering()
{
	rd = new Render(*sp, shade_back);

	Image img1(width, height), img2(width, height), img3(width, height);
	if (output_file) {
		rd->Phong_rendering(img1);
		img1.SaveTGA(output_file);
	}
	if (depth_file) {
		rd->depth_rendering(img2, depth_min, depth_max);
		img2.SaveTGA(depth_file);
	}
	if (normal_file) {
		rd->normal_rendering(img3);
		img3.SaveTGA(normal_file);
	}

}

int main(int argc,char **argv) {
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")) {
			i++; assert(i < argc);
			width = atoi(argv[i]);
			i++; assert(i < argc);
			height = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) {
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth")) {
			i++; assert(i < argc);
			depth_min = atof(argv[i]);
			i++; assert(i < argc);
			depth_max = atof(argv[i]);
			i++; assert(i < argc);
			depth_file = argv[i];
		}
		else if (!strcmp(argv[i], "-normals")) {
			i++; assert(i < argc);
			normal_file = argv[i];
		}
		else if (!strcmp(argv[i], "-shade_back")) {
			shade_back = true;
		}
		else if (!strcmp(argv[i], "-gui")) {
			gui = true;
		}
		else if (!strcmp(argv[i], "-tessellation")) {
			i++; assert(i < argc);
			theta_step = atoi(argv[i]);
			i++; assert(i < argc);
			fhi_step = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-gouraud")) {
			gouraud = true;
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}

	
	sp = new SceneParser(input_file);

	//Render rd(sp,shade_back);
	if (gui) {
		GLCanvas glc;
		glc.initialize(sp, glRendering);
	}
	else {
		glRendering();
	}
	


	


	return 0;
}
// ========================================================
// ========================================================