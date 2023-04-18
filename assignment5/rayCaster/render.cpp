#include"render.h"
#include"rayTree.h"
#include<iostream>
using namespace std;

extern bool griduse;
extern bool gridvisulize;

Render::Render(SceneParser* scene, int max_bb, float cutoff_weight, bool sd, bool sdbk, int nx, int ny, int nz,RayTree *raytree) :shade_back(sdbk),
max_b(max_bb), cutoff_w(cutoff_weight), shadows(sd)
{
	camera = scene->getCamera();
	group = scene->getGroup();
	backgroundcolor = scene->getBackgroundColor();
	//backgroundcolor /= 255;
	this->scene = scene;
	//cout << "sub1" << endl;
	if(griduse) g = new Grid(group->getBoundingbox(), nx, ny, nz,raytree);
	else 
		g = NULL;
	//cout << "sub2" << endl;
	raytracer = new RayTracer(scene, g, gridvisulize,max_b, cutoff_w, NULL, shadows, shade_back,g);
}


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
}

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
			/*Vec3f dir = r.getDirection();
			cout << dir << endl;*/
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
			/*Vec3f dir = r.getDirection();
			cout << dir << endl;*/
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
}

void Render::Grid_rendering(Image& img) {
	//group->insertIntoGrid(g,NULL);
	//g->print();
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
			/*Vec3f dir = r.getDirection();
			cout << dir << endl;*/
			Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
			color = raytracer->traceRay(r, camera->getTMin(), 0, 1, 1.0, h);
			if (h.getMaterial() != NULL)
				img.SetPixel(i, j, color);
			else
				img.SetPixel(i, j, backgroundcolor);

		}
	
}


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
			/*Vec3f dir = r.getDirection();
			cout << dir << endl;*/
			Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
			color = raytracer->traceRay(r, camera->getTMin() ,0,1,1,h);
			if(h.getMaterial()!=NULL)
				img.SetPixel(i, j, color);
			else
				img.SetPixel(i, j, backgroundcolor);
			
		}
}


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