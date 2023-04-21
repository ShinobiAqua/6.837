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
#include"raytracing_stats.h"
#include"sampler.h"
#include"filter.h"
#include"film.h"

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
bool gridvisualize = false;
bool STATS = false;


//默认使用UniformSampler 采样点数为1/pixol
//默认的filter为BoxFilter,半径为0.5个pixol
//0:UniformSampler,1:RandomSampler
//2:JitteredSampler
int sampler_choice = 0;
int sample_nums = 1;


//0: BoxFilter,1:TentFilter
//2: GaussianFilter
int filter_choice = 0;
float arg_of_filter = 0.5;

bool Render_samples = false;
int zoom_factor_rsamples = 0;
char* Render_samples_filename = NULL;

bool Render_filter = false;
int zoom_factor_rfilter = 0;
char* Render_filter_filename = NULL;

SceneParser* sp=NULL;
Render *rd=NULL;
Sampler* sampler = NULL;
Filter* filter = NULL;
Film* film = NULL;
Grid* g = NULL;



void stats_Start(int width,int height,Grid* gg) {
	if (STATS) {
		if (griduse)
			RayTracingStats::Initialize(width, height, gg->getBoundingBox(),
				gg->x(), gg->y(), gg->z());
		else
			RayTracingStats::Initialize(width, height, NULL, 0, 0, 0);
	}
}
void stats_End() {
	if (STATS)
	{
		RayTracingStats::PrintStatistics();
	}
}


void glRendering()
{
	Image img1(width, height), img2(width, height), img3(width, height);
	Grid* gg = rd->getGrid();
	if ((output_file!=NULL) || Render_filter || Render_samples) {
		stats_Start(width, height, gg);
		rd->Grid_rendering(img1);
		stats_End();
		if (output_file) {
			//stats_Start(width, height, gg);
			//rd->Grid_rendering(img1);
			img1.SaveTGA(output_file);
			//stats_End();
		}
		if (Render_filter)
			film->renderFilter(Render_filter_filename, zoom_factor_rfilter, filter);
		if (Render_samples)
			film->renderSamples(Render_samples_filename, zoom_factor_rsamples);
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
	Ray r = (rd->getCamera())->generateRay(Vec2f(x, y));
	Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
	float t = (rd->getCamera())->getTMin();
	raytracer->traceRay(r, t, 0 , 1, 1.0, h);
	//cout << h.getT() << endl;
	//raytracer->traceRay(r, rd->getCamera()->getTMin(), bounces, cutoff_w, 1.0, h);
	RayTree::SetMainSegment(r, 0, h.getT());

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
			gridvisualize = true;
		}
		else if (!strcmp(argv[i], "-stats")) {
			STATS = true;
		}
		else if (!strcmp(argv[i], "-uniform_samples")) {
			sampler_choice = 0;
			i++; assert(i < argc);
			sample_nums = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-random_samples")) {
			sampler_choice = 1;
			i++; assert(i < argc);
			sample_nums = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-jittered_samples")) {
			sampler_choice = 2;
			i++; assert(i < argc);
			sample_nums = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-box_filter")) {
			filter_choice = 0;
			i++; assert(i < argc);
			arg_of_filter = atof(argv[i]);
		}
		else if (!strcmp(argv[i], "-tent_filter")) {
			filter_choice = 1;
			i++; assert(i < argc);
			arg_of_filter = atof(argv[i]);
		}
		else if (!strcmp(argv[i], "-gaussian_filter")) {
			filter_choice = 2;
			i++; assert(i < argc);
			arg_of_filter = atof(argv[i]);
		}
		else if (!strcmp(argv[i], "-render_samples")) {
			Render_samples = true;
			i++; assert(i < argc);
			Render_samples_filename = argv[i];
			i++; assert(i < argc);
			zoom_factor_rsamples = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-render_filter")) {
			Render_filter = true;
			i++; assert(i < argc);
			Render_filter_filename = argv[i];
			i++; assert(i < argc);
			zoom_factor_rfilter = atoi(argv[i]);
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}



	//parse sampler
	if (sampler_choice == 0) {
		sampler = new UniformSampler(sample_nums);
	}
	else if (sampler_choice == 1) {
		sampler = new RandomSampler();
	}
	else if (sampler_choice == 2) {
		sampler = new JitteredSampler(sample_nums);
	}

	//parse filter
	if (filter_choice == 0) {
		filter = new BoxFilter(arg_of_filter);
	}
	else if (filter_choice == 1) {
		filter = new TentFilter(arg_of_filter);
	}
	else if (filter_choice == 2) {
		filter = new GaussianFilter(arg_of_filter);
	}

	film = new Film(width, height, sample_nums);



	sp = new SceneParser(input_file);
	rd = new Render(sp,max_b,cutoff_w,shadows,shade_back,nx,ny,nz,film,sampler,filter);
	


	if (griduse) {
		g = rd->getGrid();
		sp->getGroup()->insertIntoGrid(g, NULL);
	}


	if (gui) {
		GLCanvas glc;
		glc.initialize(sp, glRendering,traceRayfunc,g,gridvisualize);
	}
	else {
		glRendering();
	}
	

	

	return 0;
}
// ========================================================
// ========================================================