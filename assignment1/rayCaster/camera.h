#ifndef _CAMERA_H
#define _CAMERA_H
#include"ray.h"
#include"vectors.h"
class Camera {
public:
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() const = 0;
private:
};


class OrthographicCamera :public Camera {
public:
	virtual Ray generateRay(Vec2f point);
	OrthographicCamera(Vec3f center,Vec3f direction,Vec3f up,float sz):size(sz) {
		this->center = center;
		this->direction = direction;
		this->direction.Normalize();
		float prep = this->direction.Dot3(up);
		Vec3f ms = this->direction * prep;
		this->up = up - ms;
		this->up.Normalize();
		Vec3f::Cross3(this->horizontal, this->direction,this->up);
		this->horizontal.Normalize();
	}
	virtual float getTMin() const {
		return -FLT_MAX;
	}
private:
	Vec3f up, horizontal, direction,center;
	float size;
};
#endif