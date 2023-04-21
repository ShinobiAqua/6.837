#ifndef _TRANSFORM_H
#define _TRANSFORM_H
#include"object3d.h"
#include"matrix.h"
#include<iostream>

using namespace std;
class Grid;
class Matrix;
class BoundingBox;

class Transform :public Object3D {
public:
	Transform(Matrix& m, Object3D* o) :obj(o) {
		M = m;
		M_ = m;
		M_.Inverse();
		M_T = M_;
		M_T.Transpose();
		BoundingBox* boxx = obj->getBoundingbox();
		BoundingBox* box;
		if (boxx != NULL) {
				Vec3f _min, _max;
				_min = boxx->getMin();
				_max = boxx->getMax();
				Vec3f v[8];
				float a[6];
				_min.Get(a[0], a[2], a[4]);
				_max.Get(a[1], a[3], a[5]);
				//底部顺时针 v0 v2 v6 v4
				//顶部顺时针 v1 v3 v7 v5
				int ct = 0;
				for (int i = 0; i < 2; i++)
					for (int j = 2; j < 4; j++)
						for (int k = 4; k < 6; k++) {
							v[ct++].Set(a[i], a[j], a[k]);
						}
				for (int i = 0; i < 8; i++)
					m.Transform(v[i]);
				setBoundingBox(v[0],v[0]);
				box = getBoundingbox();
				for (int i = 1; i < 8; i++)
					box->Extend(v[i]);
		}
	}
	bool intersect(const Ray& r, Hit& h, float tmin);
	void paint();
	void insertIntoGrid(Grid* g, Matrix* M);
	void flattern(vector<Object3D*>& arr, Matrix* m);
private:
	Matrix M;
	Matrix M_;
	Matrix M_T;
	Object3D* obj;
};
#endif