#define _USE_MATH_DEFINES
#include"sphere.h"
#include<cmath>
#include<iostream>
#include<vector>
#include<cmath>
#include"vectors.h"
#include"grid.h"
#include"raytracing_stats.h"
#include"transform.h"

extern int theta_step, fhi_step;
extern bool gouraud;

using namespace std;
//若有更新则返回true
bool Sphere::intersect(const Ray &r,Hit &h,float tmin){

	RayTracingStats::IncrementNumIntersections();
	/*Vec3f tmp1, tmp2;
	tmp1 = r.getOrigin();
	tmp2 = r.getDirection();
	cout << tmp1 <<tmp2 << endl;*/
	float A, B, C;
	Vec3f orig = r.getOrigin();
	Vec3f d = r.getDirection();
	orig -= center;
	A = d.Dot3(d);
	B = 2 * orig.Dot3(d);
	C = orig.Dot3(orig) - radius * radius;
	float dert = B * B - 4 * A * C;
	//no intersect
	if (dert < 0) return false;
	float t1, t2 ,t;
	float sqdert = sqrt(dert);
	t1 = (sqdert - B) / (2 * A);
	t2 = (-B - sqdert) / (2 * A);
	
	//both less than tmin
	if (t1 < tmin && t2 < tmin)  return false; 
	//at least one intersect
	t = t1;
	if (t2 >= tmin && t2 < t1)
		t = t2;
	float th = h.getT();
	// if less ,then update
	//2760.3
	if (t < th) {
		Vec3f normal = r.pointAtParameter(t);
		normal -= center;
		//cout << A << ' ' << B << ' ' << C <<' '<<dert<<' '<<sqdert<< endl;
		//cout << orig<<" "<<d<<" "<<normal <<t1<<' '<<t2<<' '<<radius<< endl;
		normal.Normalize();
		h.set(t, material,normal, r);
		return true;
	}
	return false;
}

void Sphere::paint() {

	material->glSetMaterial();
	float fhi_mini, theta_mini;
	fhi_mini = M_PI / fhi_step;
	theta_mini = M_PI*2 / theta_step;
	if (V_empty()) {
		float x, y, z;
		Vec3f v;
		float tt = 0;
		float fi = -M_PI;
		for (int i = 0; i <= fhi_step; i++) {
			tt = 0;
			for (int j = 0; j <= theta_step; j++)
			{
				z = cos(fi);
				x = sin(fi) * cos(tt);
				y = sin(fi) * sin(tt);
				v.Set(x, y, z);

				normal.push_back(v);
				v *= radius;
				v += center;
				push(v);
				tt += theta_mini;
				//cout << v << endl;
			}
			fi += fhi_mini;
		}
			
	}


	
	Vec3f v1, v2, v3, v4;
	Vec3f e1, e2, norm;
	Vec3f norm1, norm2, norm3, norm4;
	if(gouraud) 
		glShadeModel(GL_SMOOTH);

	for (int j = 0; j < fhi_step; j++)
		for(int i = 0; i < theta_step ;i++)
		{
			
			v1 = getV(j * (theta_step + 1) + i);
			v2 = getV(j * (theta_step + 1) + i + 1);
			v3 = getV((j + 1) * (theta_step + 1) + i + 1);
			v4 = getV((j + 1) * (theta_step + 1) + i);
			e1 = v1 - v3;
			e2 = v4 - v2;
			Vec3f::Cross3(norm, e2, e1);
			norm.Normalize();
			
			
			if (!gouraud) {
				glBegin(GL_QUADS);
				glNormal3f(norm.x(), norm.y(), norm.z());
				glVertex3f(v1.x(), v1.y(), v1.z());
				glVertex3f(v2.x(), v2.y(), v2.z());
				glVertex3f(v3.x(), v3.y(), v3.z());
				glVertex3f(v4.x(), v4.y(), v4.z());
				glEnd();
			}
			else {
				norm1 = normal[j * (theta_step + 1) + i];
				norm2 = normal[j * (theta_step + 1) + i + 1];
				norm3 = normal[(j + 1) * (theta_step + 1) + i + 1];
				norm4 = normal[(j + 1) * (theta_step + 1) + i];
				glBegin(GL_QUADS);
				glNormal3f(norm1.x(), norm1.y(), norm1.z());
				glVertex3f(v1.x(), v1.y(), v1.z());
				glNormal3f(norm2.x(), norm2.y(), norm2.z());
				glVertex3f(v2.x(), v2.y(), v2.z());
				glNormal3f(norm3.x(), norm3.y(), norm3.z());
				glVertex3f(v3.x(), v3.y(), v3.z());
				glNormal3f(norm4.x(), norm4.y(), norm4.z());
				glVertex3f(v4.x(), v4.y(), v4.z());
				glEnd();
			}
			//cout << v1 << v2 << v3 << v4 << endl;
		}

	
	
}

void Sphere::insertIntoGrid(Grid* g, Matrix* M) {
	if (M != NULL)
	{
		this->Object3D::insertIntoGrid(g, M);
		return;
	}
	int nx, ny, nz;
	nx = g->x();
	ny = g->y();
	nz = g->z();
	for (int i = 0; i < nx; i++) {
		for (int j = 0; j < ny; j++) {
			for (int k = 0; k < nz; k++) {
				//计算格点i,j,k是否在几何体内部
				//获得i,j,k的world坐标
				Vec3f point1 = g->getPoint(i, j, k);
				Vec3f point2 = g->getPoint(i + 1, j + 1, k + 1);
				Vec3f point = 0.5 * point1 + 0.5 * point2;
				//在内部
				point -= center;
				if ((point.Length() - sqrt(3) * 0.5 * (point2.x() - point1.x())) <= radius) {
					g->setVoxel(i, j, k, true);
					g->setObj(i, j, k, this);
				}
				//cout << sqrt(3) * 0.5 * (point2.x() - point1.x()) << endl;
			}
		}
	}
}


