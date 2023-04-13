#ifndef _TRANSFORM_H
#define _TRANSFORM_H
#include"object3d.h"
#include"matrix.h"
#include<iostream>
using namespace std;

class Transform :public Object3D {
public:
	Transform(Matrix& m, Object3D* o) :obj(o) {
		M = m;
		M_ = m;
		M_.Inverse();
		M_T = M_;
		M_T.Transpose();
	}
	bool intersect(const Ray& r, Hit& h, float tmin);
	void paint();
private:
	Matrix M;
	Matrix M_;
	Matrix M_T;
	Object3D* obj;
};
#endif