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
using namespace std;
char* input_file = NULL;
int width = 100;
int height = 100;
char* output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char* depth_file = NULL;

// sample command line:
// raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga
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
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}


	SceneParser sp(input_file);
	Camera* cmr = sp.getCamera();
	Group* grp = sp.getGroup();
	Vec3f backgroudcolor = sp.getBackgroundColor();
	backgroudcolor /= 255;
	Vec3f tmpcolor;
	Image img(width, height);
	Image dptimg(width, height);
	Vec2f s;
	Vec2f tmp;
	Vec2f point;
	int scalen;

	if (height > width)
	{
		s.Set(float(height - width) / 2, 0);
		scalen = height-1;
	}
	else
	{
		s.Set(0, float(width - height) / 2);
		scalen = width-1;
	}
	
	for(int i=0;i<width;i++)
		for (int j = 0; j < height; j++) {
			tmp.Set(i, j);
			Vec2f::Add(point, s, tmp);
			point /= scalen;
			//cout << point.x()<<' '<<point.y() << endl;
			Ray r = cmr->generateRay(point);
			Hit h(FLT_MAX, NULL);
			grp->intersect(r, h, cmr->getTMin());
			Material* mtr = h.getMaterial();
			if(mtr){
				img.SetPixel(i, j, mtr->getDiffuseColor());
				float t = h.getT();
				if (t < depth_min) t=depth_min;
				if (t > depth_max) t=depth_max;
				float gry = (depth_max-t)/ float(depth_max - depth_min);
				//cout << gry << endl;
				dptimg.SetPixel(i, j, Vec3f(gry,gry,gry));
			}
			else{
				img.SetPixel(i, j, backgroudcolor);
				dptimg.SetPixel(i, j, Vec3f(0,0,0));
			}
		}

	/*for(int i=0;i<width;i++)
		for (int j = 0; j < height; j++) {
			Vec3f aa= dptimg.GetPixel(i, j);
			if(aa.x()!=0&&aa.y()!=0&&aa.z()!=0)
			cout << aa << endl;
		}*/
	img.SaveTGA(output_file);
	dptimg.SaveTGA(depth_file);



	return 0;
}
// ========================================================
// ========================================================