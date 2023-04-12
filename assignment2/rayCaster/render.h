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
	Vec3f getColor(Hit& h) {
		Vec3f color(0,0,0);
		Vec3f objcolor = (h.getMaterial())->getDiffuseColor();
		Vec3f tmpcolor;
		Vec3f dir;
		Vec3f normal = h.getNormal();
		float coh;
		color += scene->getAmbientLight()*objcolor;
		for (int i = 0; i < scene->getNumLights(); i++) {
			Light* light = scene->getLight(i);
			float dtl = 0;
			light->getIllumination(h.getIntersectionPoint(), dir, tmpcolor,dtl);
			coh = normal.Dot3(dir);
			if (shade_back&&coh<0)
				coh = -coh;
			coh = max(coh, float(0));
			color += coh * objcolor * tmpcolor;
		}
	
		return color;
	}

private:
	Render() { assert(0); };
	Camera* camera;
	Group* group;
	Vec3f backgroundcolor;
	const SceneParser* scene;
	bool shade_back;
};






#endif