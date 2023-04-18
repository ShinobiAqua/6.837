#include"plane.h"
#include"matrix.h"

const int BIG = 80000;
bool Plane::intersect(const Ray& r, Hit& h, float tmin) {
	Vec3f R0 = r.getOrigin();
	Vec3f Rd = r.getDirection();
	Vec3f P0 = normal * d;
	float f1, f2;
	P0 = P0 - R0;
	f1 = P0.Dot3(normal);
	f2 = Rd.Dot3(normal);
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
	Vec3f::Cross3(b2, b1, normal);
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