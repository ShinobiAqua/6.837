#include"triangle.h"



Vec3f Triangle::BCcoord(Vec3f point) {
	Vec3f e1, e2;
	e1 = b - a;
	e2 = c - a;
	Vec4f cox;
	Vec3f ans;
	Vec3f tmp = point - a;
	float c1, c2;
	c1 = tmp.Dot3(e1);
	c2 = tmp.Dot3(e2);
	cox.Set(c1, c2, 0, 0);
	M.Transform(cox);
	ans.Set(1 - cox.x() - cox.y(), cox.x(), cox.y());
	return ans;
}
bool Triangle::intersect(const Ray& r, Hit& h, float tmin) {
	Vec3f R0 = r.getOrigin();
	Vec3f Rd = r.getDirection();
	Vec3f tmp = a - R0;
	float f1, f2;
	f1 = tmp.Dot3(normal);
	f2 = Rd.Dot3(normal);
	float t = f1 / f2;
	if (t < tmin) return false;
	Vec3f point = r.pointAtParameter(t);
	Vec3f bccoord = BCcoord(point);
	if (!inTriangle(bccoord)) return false;
	if (t < h.getT())
	{
		h.set(t, m, normal, r);
		return true;
	}
	return false;
}

void Triangle::paint()
{
	m->glSetMaterial();
	glBegin(GL_TRIANGLES);
	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(a.x(), a.y(), a.z());
	glVertex3f(b.x(), b.y(), b.z());
	glVertex3f(c.x(), c.y(), c.z());
	glEnd();
	
}