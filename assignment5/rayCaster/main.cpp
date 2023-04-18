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
#include"rayTree.h"
#include"ray.h"


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
int max_b = 1;
bool shadows;
float cutoff_w = 0.01;
int nx, ny, nz;
bool griduse = false;
bool gridvisulize = false;
SceneParser* sp;
Render *rd;
RayTree raytree;

void glRendering()
{
	(rd->getRayTracer())->trun_off();
	Image img1(width, height), img2(width, height), img3(width, height);
	if (output_file) {
		if (!gridvisulize) {
			rd->Phong_rendering_new(img1);
			img1.SaveTGA(output_file);
		}
		else {
			rd->Grid_rendering(img1);
			img1.SaveTGA(output_file);
		}
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

void traceRayfunc(float x,float y) {
	RayTracer *raytracer = rd->getRayTracer();
	raytracer->setRaytree(&raytree);
	raytracer->trun_on();
	Ray r = (rd->getCamera())->generateRay(Vec2f(x, y));
	Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
	float t = (rd->getCamera())->getTMin();
	raytracer->traceRay(r, t, 0 , 1, 1.0, h);
	//cout << h.getT() << endl;
	//raytracer->traceRay(r, rd->getCamera()->getTMin(), bounces, cutoff_w, 1.0, h);
	raytree.SetMainSegment(r, 0, h.getT());
	raytracer->trun_off();
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
		else if (!strcmp(argv[i], "-shadows")) {
			shadows = true;
		}
		else if (!strcmp(argv[i], "-bounces")) {
			i++; assert(i < argc); 
			max_b = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-weight")) {
			i++; assert(i < argc);
			cutoff_w = atof(argv[i]);
		}
		else if (!strcmp(argv[i],"-grid")) {
			i++; assert(i < argc);
			nx = atoi(argv[i]);
			i++; assert(i < argc);
			ny = atoi(argv[i]);
			i++; assert(i < argc);
			nz = atoi(argv[i]);
			griduse = true;
		}
		else if (!strcmp(argv[i],"-visualize_grid")) {
			gridvisulize = true;
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}
	
	//cout << 11112132 << endl;
	sp = new SceneParser(input_file);
	
	RayTree raytree;
	//cout << 111121 << endl;
	rd = new Render(sp,max_b,cutoff_w,shadows,shade_back,nx,ny,nz,&raytree);

	Grid* g = NULL;
	//cout << 11111 << endl;
	if (griduse) {
		g = rd->getGrid();
		sp->getGroup()->insertIntoGrid(g,NULL);
		//g->print();
		//cout << 111 << endl;
	}

	//Render rd(sp,shade_back);
	if (gui) {
		//cout << 22222 << endl;
		GLCanvas glc;
		
		glc.initialize(sp, glRendering,traceRayfunc,g,gridvisulize);
	}
	else {
		glRendering();
	}
	

	delete g;
	


	return 0;
}
// ========================================================
// ========================================================