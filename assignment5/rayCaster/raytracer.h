#ifndef _RAYTRACER_H
#define _RAYTRACER_H
#include<assert.h>
#include"ray.h"
#include"hit.h"
#include"material.h"
#include"scene_parser.h"
#include"light.h"
#include"group.h"
#include"camera.h"
#include<cmath>
#include"grid.h"
#include"rayTree.h"
#define OPTION_TRANSPARENT_SHADOWS 1



class RayTracer {
public:
	RayTracer(SceneParser* sc,Grid *gr,bool grdv, int max_b = 3,
		float cutoff_w=0.01,RayTree* rayt=NULL,bool sds=false,bool sdbc=false,bool sit=false):
	s(sc),max_bounces(max_b),raytree(rayt),cutoff_weight(cutoff_w),
	shadows(sds),shade_back(sdbc),store_in_tree(sit),g(gr),gridvisualize(gr){
	}


	Vec3f traceRay(Ray& ray, float tmin, int bounces, float weight,
		float indexOfRefraction, Hit& hit);


	Vec3f mirrorDirection(const Vec3f& normal, const Vec3f& incoming);


	bool transmittedDirection(const Vec3f& normal, const Vec3f& incoming,
		float index_i, float index_t, Vec3f& transmitted);


	void trun_off() { store_in_tree = false; }


	void trun_on() {
		if (raytree != NULL) store_in_tree = true;
		else store_in_tree = false;
	}


	Vec3f getPhongColor(Ray& r, Hit& h);


	void setRaytree(RayTree* rayt) {
		raytree = rayt;
	}


private:
	RayTracer() {assert(0);}
	SceneParser* s;
	int max_bounces;
	float cutoff_weight;
	bool shadows;
	bool shade_back;
	RayTree *raytree;
	bool store_in_tree;
	Grid* g;
	bool gridvisualize;
};


#endif