#ifndef _TRIANGLE_H
#define _TRIANGLE_H
#include"object3d.h"
#include"material.h"
#include"matrix.h"
#include<assert.h>
#include<iostream>
using namespace std;

class Triangle :public Object3D {
public:
	Triangle(Vec3f x,Vec3f y,Vec3f z,Material *mat):a(x),b(y),c(z),m(mat){
		Vec3f v1, v2;
		v1 = a-c;
		v2 = b-a;
		v1.Normalize();
		v2.Normalize();
		assert(Vec3f::Abs(v1) != Vec3f::Abs(v2));
		Vec3f::Cross3(normal, v1, v2);
		normal.Normalize();

		Vec3f e1, e2;
		e1 = b - a;
		e2 = c - a;
		M.SetToIdentity();
		M.Set(0, 0, e1.Dot3(e1));
		M.Set(1, 0, e1.Dot3(e2));
		M.Set(0, 1, e1.Dot3(e2));
		M.Set(1, 1, e2.Dot3(e2));
		
		M.Inverse(1e-9);
	}
	Vec3f BCcoord(Vec3f point);
	bool inTriangle(Vec3f a) {
		if (a.x() < 0 || a.x() > 1) return false;
		if (a.y() < 0 || a.y() > 1) return false;
		if (a.z() < 0 || a.z() > 1) return false;
		return true;
	}
	bool intersect(const Ray& r, Hit& h, float tmin);
	void paint();
private:
	Vec3f a, b, c;
	Vec3f normal;
	Material* m;
	Matrix M;
};

#endif