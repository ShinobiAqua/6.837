#ifndef _PLANE_H
#define _PLANE_H
#include"object3d.h"
#include"material.h"
#include<assert.h>
#include"grid.h"

class Grid;
class Plane :public Object3D {
public:
	bool intersect(const Ray& r, Hit& h, float tmin);
	Plane(Vec3f &n,float dis,Material *mat):normal(n),d(dis),m(mat){
	}
	Plane(Vec3f a1, Vec3f a2, Vec3f a3) {
		Vec3f v1 = a2 - a1;
		Vec3f v2 = a3 - a1;
		v1.Normalize();
		v2.Normalize();
		assert(Vec3f::Abs(v1) != Vec3f::Abs(v2));
		Vec3f::Cross3(normal,v1,v2);
		normal.Normalize();
		d = normal.Dot3(a1);
		if (d < 0) {
			d = -d;
			normal.Negate();
		}
	}
	void paint();
	void insertIntoGrid(Grid* g, Matrix* m){
		g->addPlanes(this, m);
		return;
	}
	/*void flattern(vector<Object3D*>& arr, Matrix* m) {
		this->Object3D::flattern(arr, m);
	}*/
private:
	Vec3f normal;
	float d;
	Material* m;

};


#endif