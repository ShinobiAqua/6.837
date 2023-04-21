#include"render.h"
#include"rayTree.h"
#include<iostream>
#include"raytracing_stats.h"
#include"sample.h"
#include"film.h"
#include"sampler.h"
#include"filter.h"
using namespace std;

extern bool griduse;
extern bool gridvisualize;

Render::Render(SceneParser* scene, int max_bb,
	float cutoff_weight, bool sd,
	bool sdbk, int nx, int ny, int nz,
	Film* flm,Sampler *spl,Filter* flt) :shade_back(sdbk),
	max_b(max_bb), cutoff_w(cutoff_weight), shadows(sd),
	film(flm),sampler(spl),filter(flt)
{
	camera = scene->getCamera();
	group = scene->getGroup();
	backgroundcolor = scene->getBackgroundColor();
	//backgroundcolor /= 255;
	this->scene = scene;
	//cout << "sub1" << endl;
	if(griduse) g = new Grid(group->getBoundingbox(), nx, ny, nz,gridvisualize);
	else 
		g = NULL;
	//cout << "sub2" << endl;
	raytracer = new RayTracer(scene, g, gridvisualize,griduse,max_b, cutoff_w,shadows, shade_back);
}


// new version adpt to sample
void Render::depth_rendering(Image& img, float depth_min, float depth_max) {
	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f zz_Point;
	Vec2f point;
	Vec2f offset;
	Vec3f color;
	int scalen;
	int num_samples = film->getNumSamples();
	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height - 1;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width - 1;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			zz_Point.Set(i, j);
			zz_Point += start;
			//cout << point.x()<<' '<<point.y() << endl;
			for (int k = 0; k < num_samples; k++) {
				offset = sampler->getSamplePosition(k);
				point = zz_Point + offset;
				point /= scalen;
				Ray r = camera->generateRay(point);
				Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
				bool ismx;
				if (!griduse)
					ismx = group->intersect(r, h, camera->getTMin());
				else
					ismx = g->intersect(r, h, camera->getTMin());
				if (ismx) {
					float t = h.getT();
					float gry = (depth_max - t) / float(depth_max - depth_min);
					color.Set(gry, gry, gry);
					film->setSample(i, j, k, offset, color);
				}
				else {
					film->setSample(i, j, k, offset, Vec3f(0,0,0));
				}
			}
		}
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			color = filter->getColor(i, j, film);
			img.SetPixel(i, j, color);
		}
}



//new version adpt to sample
void Render::normal_rendering(Image& img) {
	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f zz_Point;
	Vec2f point;
	Vec2f offset;
	Vec3f color;
	int scalen;
	int num_samples = film->getNumSamples();
	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height - 1;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width - 1;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			zz_Point.Set(i, j);
			zz_Point += start;
			for (int k = 0; k < num_samples; k++) {
				offset = sampler->getSamplePosition(k);
				//printf("i: %d j: %d k: %d  x: %f y: %f", i, j, k, offset.x(), offset.y());
				point = zz_Point + offset;
				point /= scalen;
				Ray r = camera->generateRay(point);
				Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
				bool ismx;
				if (!griduse)
					ismx = group->intersect(r, h, camera->getTMin());
				else
					ismx = g->intersect(r, h, camera->getTMin());
				if (ismx) {
					color = h.getNormal();
					color.Abs();
					film->setSample(i, j, k, offset, color);
				}
				else {
					film->setSample(i, j, k, offset, backgroundcolor);
				}
			}
		}
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			color = filter->getColor(i, j, film);
			img.SetPixel(i, j, color);
		}
}

//new version adapt to sample


void Render::Grid_rendering(Image& img) {

	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f point;
	Vec2f zz_Point;//the coordinate of pixol ,最左下角点坐标
	Vec3f color;
	Vec2f offset;
	//Sample sample;
	int scalen;
	int num_samples = film->getNumSamples();

	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			zz_Point.Set(i, j);
			zz_Point += start;
			
			for (int k = 0; k < num_samples; k++) {
				offset = sampler->getSamplePosition(k);
				point = zz_Point + offset;
				point /= scalen;
				Ray r = camera->generateRay(point);
				Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
				color = raytracer->traceRay(r, camera->getTMin(), 0, 1, 1.0, h);
				if (h.getMaterial() != NULL)
					film->setSample(i, j, k, offset, color);
				else
					film->setSample(i, j, k, offset, backgroundcolor);
			}
			//cout << color << endl;
			
		}	
	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			color = filter->getColor(i, j, film);
			img.SetPixel(i, j, color);
		}
}


Vec3f Render::getPhongColor(const Ray& r, Hit& h)
{
	Vec3f color(0, 0, 0);
	Vec3f objcolor = (h.getMaterial())->getDiffuseColor();
	Vec3f tmpcolor;
	Vec3f dir;
	Vec3f normal = h.getNormal();
	float coh= r.getDirection().Dot3(normal);
	Material* material = h.getMaterial();
	if (shade_back || r.getDirection().Dot3(normal) <= 0)
		color += scene->getAmbientLight() * objcolor;
	
	for (int i = 0; i < scene->getNumLights(); i++) {
		Light* light = scene->getLight(i);
		float dtl = 0;
		light->getIllumination(h.getIntersectionPoint(), dir, tmpcolor, dtl);
		if (shade_back && coh > 0) {
			normal.Negate();
			h.setNorm(normal);
		}
		color += material->Shade(r, h, dir, tmpcolor);
	}
	return color;
}



//old version
/*
void Render::Phong_rendering_new(Image& img) {
	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f point;
	Vec3f color;
	int scalen;
	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height - 1;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width - 1;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			point.Set(i, j);
			point += start;
			point /= scalen;
			//cout << 1 << endl;
			//cout << point.x()<<' '<<point.y() << endl;
			Ray r = camera->generateRay(point);
			Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
			color = raytracer->traceRay(r, camera->getTMin() ,0,1,1,h);
			RayTracingStats::IncrementNumNonShadowRays();
			if(h.getMaterial()!=NULL)
				img.SetPixel(i, j, color);
			else
				img.SetPixel(i, j, backgroundcolor);

		}
}
*/


//old version
/*
Vec3f Render::getDeffuseColor(Hit& h) {
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
*/


//old version
/*
void Render::diffuse_rendering(Image& img) {
	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f point;
	Vec3f color;
	int scalen;
	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height - 1;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width - 1;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			point.Set(i, j);
			point += start;
			point /= scalen;
			//cout << 1 << endl;
			//cout << point.x()<<' '<<point.y() << endl;
			Ray r = camera->generateRay(point);

			Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
			bool ismx = group->intersect(r, h, camera->getTMin());

			if (ismx) {
				color = getDeffuseColor(h);
				img.SetPixel(i, j,color);
			}
			else {
				//cout << 1 << endl;
				img.SetPixel(i, j, backgroundcolor);
			}
		}
}
*/


//old version
/*
void Render::Phong_rendering(Image& img) {
	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f point;
	Vec3f color;
	int scalen;
	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height - 1;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width - 1;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			point.Set(i, j);
			point += start;
			point /= scalen;
			//cout << 1 << endl;
			//cout << point.x()<<' '<<point.y() << endl;
			Ray r = camera->generateRay(point);

			Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
			bool ismx = group->intersect(r, h, camera->getTMin());

			if (ismx) {
				color = getPhongColor(r,h);
				img.SetPixel(i, j, color);
			}
			else {
				//cout << 1 << endl;
				img.SetPixel(i, j, backgroundcolor);
			}
		}
}*/


//odd version of normal_rendering
/*
void Render::normal_rendering(Image& img) {
	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f point;
	int scalen;
	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height - 1;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width - 1;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			point.Set(i, j);
			point += start;
			point /= scalen;
			//cout << point.x()<<' '<<point.y() << endl;
			Ray r = camera->generateRay(point);
			Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
			bool ismx = group->intersect(r, h, camera->getTMin());

			if (ismx) {
				Vec3f color = h.getNormal();
				color.Abs();
				img.SetPixel(i, j, color);
			}
			else {
				img.SetPixel(i, j, backgroundcolor);
			}
		}
}*/


//ord version
/*
void Render::basic_rendering(Image& img) {
	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f point;
	int scalen;
	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height - 1;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width - 1;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			point.Set(i, j);
			point += start;
			point /= scalen;
			//cout << point.x()<<' '<<point.y() << endl;
			Ray r = camera->generateRay(point);
			Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
			bool ismx = group->intersect(r, h, camera->getTMin());

			if (ismx) {
				Material* mtr = h.getMaterial();
				img.SetPixel(i, j, mtr->getDiffuseColor());
			}
			else {
				img.SetPixel(i, j, backgroundcolor);
			}
		}
}
*/

//ord version
/*
void Render::depth_rendering(Image& img,float depth_min,float depth_max) {
	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f point;
	int scalen;
	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height - 1;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width - 1;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			point.Set(i, j);
			point += start;
			point /= scalen;
			//cout << point.x()<<' '<<point.y() << endl;
			Ray r = camera->generateRay(point);
			Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
			bool ismx = group->intersect(r, h, camera->getTMin());
			if (ismx) {
				float t = h.getT();
				float gry = (depth_max - t) / float(depth_max - depth_min);
				img.SetPixel(i, j, Vec3f(gry, gry, gry));
			}
			else {
				img.SetPixel(i, j, Vec3f(0,0,0));
			}
		}
}
*/