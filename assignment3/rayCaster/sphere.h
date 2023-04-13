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
	}
	Sphere(Sphere& sp) {
		this->center = sp.center;
		this->radius = sp.radius;
		this->material = sp.material;
	}
	
	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	void paint();
private:
	Vec3f center;
	float radius;
	Material* material;
	vector<Vec3f> normal;
};
#endif