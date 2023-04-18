#ifndef _RENDER_H
#define _RENDER_H
#include<assert.h>
#include"scene_parser.h"
#include"camera.h"
#include"group.h"
#include"image.h"
#include"material.h"
#include"vectors.h"
#include"light.h"
#include"ray.h"
#include<algorithm>
#include"raytracer.h"

class RayTree;


class Render {
public:
	Render(SceneParser* scene, int max_bb,
		float cutoff_weight, bool sd,
		bool sdbk, int nx, int ny, int nz,RayTree *raytree);

	~Render() {
		delete raytracer;
		delete g;
	}
	void basic_rendering(Image& img);
	void depth_rendering(Image& img,float depth_min,float depth_max);
	void normal_rendering(Image& img);
	void diffuse_rendering(Image& img);
	void Phong_rendering(Image& img);
	void Phong_rendering_new(Image& img);
	void Grid_rendering(Image& img);
	Vec3f getDeffuseColor( Hit& h);
	Vec3f getPhongColor(const Ray& r, Hit& h);
	RayTracer* getRayTracer() { return raytracer; }
	Camera* getCamera() { return camera; }
	Grid* getGrid() { return g; }

private:
	Render() { assert(0); };
	Camera* camera;
	Group* group;
	Vec3f backgroundcolor;
	SceneParser* scene;
	bool shade_back;
	RayTracer* raytracer;
	int max_b;
	float cutoff_w;
	bool shadows;
	Grid* g;

};






#endif