#include"sphere.h"
#include<cmath>
bool Sphere::intersect(const Ray &r,Hit &h,float tmin){

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
	t1 = (sqdert - B) / 2 * A;
	t2 = (-B - sqdert) / 2 * A;
	
	//both less than tmin
	if (t1 < tmin && t2 < tmin)  return false; 
	//at least one intersect
	t = t1;
	if (t2 >= tmin && t2 < t1)
		t = t2;
	float th = h.getT();
	// if less ,then update
	if (t < th)
		h.set(t, material, r);
	return true;
}