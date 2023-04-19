#include"transform.h"
#include"matrix.h"
#include"vectors.h"
#include<iostream>
#include"ray.h"
#include"hit.h"
#include"grid.h"
using namespace std;


bool Transform::intersect(const Ray& r, Hit& h, float tmin) {

	Vec3f dir, orig;
	dir = r.getDirection();
	orig = r.getOrigin();
	Vec4f tp1(dir.x(),dir.y(),dir.z(),0);
	Vec4f tp2(orig.x(), orig.y(), orig.z(), 1);
	M_.Transform(tp1);
	M_.Transform(tp2);
	dir.Set(tp1.x(), tp1.y(), tp1.z());
	orig.Set(tp2.x(), tp2.y(), tp2.z());
	Ray rn(orig, dir);
	
	//cout << orig <<"  "<<dir<< endl;
	bool isupdate=obj->intersect(rn, h, tmin);
	if (isupdate)
	{
		Vec3f norm= h.getNormal();
		//cout << norm << endl;
		Vec4f tmp(norm.x(), norm.y(), norm.z(),0);
		M_T.Transform(tmp);
		norm.Set(tmp.x(), tmp.y(), tmp.z());
		norm.Normalize();
		h.setNorm(norm);
		h.setPoint(r, h.getT());
	}
	return isupdate;
}

void Transform::paint()
{
	glPushMatrix();
	GLfloat* glMatrix = M.glGet();
	glMultMatrixf(glMatrix);
	obj->paint();
	delete[] glMatrix;
	glPopMatrix();
}

void Transform::insertIntoGrid(Grid* g, Matrix* _M) {
	Matrix nM;
	nM.SetToIdentity();
	if (_M != NULL) nM = *_M;
	nM *= M;
	//nM.print();
	obj->insertIntoGrid(g, &nM);
	
}

void Transform::flattern(vector<Object3D*>& arr, Matrix* _M) {
	Matrix nM;
	nM.SetToIdentity();
	if (_M != NULL) nM = *_M;
	nM *= M;
	obj->flattern(arr, &nM);
}