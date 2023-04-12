#ifndef _CAMERA_H
#define _CAMERA_H
#include"ray.h"
#include"vectors.h"
#include<cmath>
#include"scene_parser.h"


class Camera {
public:
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() const = 0;
	virtual void glInit(int w, int h) = 0;
	virtual void glPlaceCamera(void) = 0;
	virtual void dollyCamera(float dist) = 0;
	virtual void truckCamera(float dx, float dy) = 0;
	virtual void rotateCamera(float rx, float ry) = 0;

private:
};


class OrthographicCamera :public Camera {
public:
	Ray generateRay(Vec2f point);
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
	void glInit(int w, int h) ;
	void glPlaceCamera(void) ;
	void dollyCamera(float dist) ;
	void truckCamera(float dx, float dy) ;
	void rotateCamera(float rx, float ry) ;


	float getTMin() const {
		return -FLT_MAX;
	}
private:
	Vec3f up, horizontal, direction,center;
	float size;
};

class PerspectiveCamera:public Camera {
public:
	Ray generateRay(Vec2f point);
	PerspectiveCamera(Vec3f center, Vec3f direction, Vec3f up, float ag):angle(ag) {
		this->center = center;
		this->direction = direction;
		this->direction.Normalize();
		float prep = this->direction.Dot3(up);
		Vec3f ms = this->direction * prep;
		this->up = up - ms;
		this->up.Normalize();
		Vec3f::Cross3(this->horizontal, this->direction, this->up);
		this->horizontal.Normalize();
		float d = 0.5 / tan(0.5 * angle);
		start = center + (-0.5)*horizontal + (-0.5) * up + d*direction;
	}
	void glInit(int w, int h);
	void glPlaceCamera(void);
	void dollyCamera(float dist);
	void truckCamera(float dx, float dy);
	void rotateCamera(float rx, float ry);

	float getTMin() const {
		return FLT_MIN;
	}
private:
	Vec3f up, horizontal, direction, center;
	Vec3f start;
	float angle;
};

#endif