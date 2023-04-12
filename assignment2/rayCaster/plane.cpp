#include"plane.h"
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