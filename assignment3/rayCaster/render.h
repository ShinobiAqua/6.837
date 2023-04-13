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

class Render {
public:
	Render(const SceneParser &scene,bool sd):shade_back(sd){
		camera = scene.getCamera();
		group = scene.getGroup();
		backgroundcolor = scene.getBackgroundColor();
		//backgroundcolor /= 255;
		this->scene = &scene;
	}
	void basic_rendering(Image& img);
	void depth_rendering(Image& img,float depth_min,float depth_max);
	void normal_rendering(Image& img);
	void diffuse_rendering(Image& img);
	void Phong_rendering(Image& img);
	Vec3f getDeffuseColor( Hit& h);
	Vec3f getPhongColor(const Ray& r, Hit& h);
	

private:
	Render() { assert(0); };
	Camera* camera;
	Group* group;
	Vec3f backgroundcolor;
	const SceneParser* scene;
	bool shade_back;
};






#endif