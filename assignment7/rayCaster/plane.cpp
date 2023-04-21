#include"plane.h"
#include"matrix.h"
#include"raytracing_stats.h"
#define GETaix(v) (v).x(),(v).y(),(v).z()
#define GETaix_add1(v) (v).x()+1,(v).y()+1,(v).z()+1
const int BIG = 80000;



bool Plane::intersect(const Ray& r, Hit& h, float tmin) {
	RayTracingStats::IncrementNumIntersections();

	Vec3f R0 = r.getOrigin();
	Vec3f Rd = r.getDirection();
	Vec3f P0 = normal * d;
	float f1, f2;
	P0 = P0 - R0;
	f1 = P0.Dot3(normal);
	f2 = Rd.Dot3(normal);
	if (f2 == 0) return false;
	float t = f1 / f2;
	if (t < tmin) return false;
	if (t < h.getT())
	{
		h.set(t, m, normal, r);
		return true;
	}
	return false;
}

void Plane::paint() {
	Vec3f v(1, 0, 0);
	if (normal == v)
		v.Set(0, 1, 0);
	Vec3f b1, b2;
	Vec3f::Cross3(b1, v, normal);
	Vec3f::Cross3(b2, normal,b1);
	Vec3f orig = normal * d;
	Vec3f tmp;
	

	m->glSetMaterial();
	
	glBegin(GL_QUADS);
	glNormal3f(normal.x(),normal.y(), normal.z());
	tmp = orig + BIG * b1 + BIG * b2;
	glVertex3f(tmp.x(), tmp.y(), tmp.z());
	tmp = orig + (-BIG) * b1 + BIG * b2;
	glVertex3f(tmp.x(), tmp.y(), tmp.z());
	tmp = orig + (-BIG) * b1 + (-BIG) * b2;
	glVertex3f(tmp.x(), tmp.y(), tmp.z());
	tmp = orig + BIG * b1 + (-BIG) * b2;
	glVertex3f(tmp.x(), tmp.y(), tmp.z());

	glEnd();
}