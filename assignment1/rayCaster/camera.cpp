#include"camera.h"
Ray OrthographicCamera::generateRay(Vec2f point) {
	Vec2f ctr(0.5, 0.5);
	point -= ctr;
	Vec3f orig;
	orig = center + up * point.y()*size + horizontal * point.x()*size;
	Ray ans(orig, direction);
	return ans;
}