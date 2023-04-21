#ifndef _SPHRER_H
#define _SPHRER_H
#include"object3d.h"
#include"material.h"
#include<vector>

class Sphere:public Object3D {
public:
	Sphere(Vec3f center, float radius, Material* material) {
		this->center = center;
		this->radius = radius;
		this->material = material;
		Vec3f _min, _max;
		_min = center - Vec3f(1., 1., 1.) * radius;
		_max = center + Vec3f(1., 1., 1.) * radius;
		setBoundingBox(_min, _max);
	}
	Sphere(Sphere& sp) {
		this->center = sp.center;
		this->radius = sp.radius;
		this->material = sp.material;
		Vec3f _min, _max;
		_min = center - Vec3f(1., 1., 1.) * radius;
		_max = center + Vec3f(1., 1., 1.) * radius;
		setBoundingBox(_min, _max);
	}
	void insertIntoGrid(Grid* g, Matrix* m);
	bool intersect(const Ray& r, Hit& h, float tmin);
	void paint();
	void flattern(vector<Object3D*>& arr, Matrix* m) {
		this->Object3D::flattern(arr, m);
	}
private:
	Vec3f center;
	float radius;
	Material* material;
	vector<Vec3f> normal;
};
#endif