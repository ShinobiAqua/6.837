#ifndef _FORCE_FIELD_H
#define _FORCE_FIELD_H
#include"vectors.h"
#include<assert.h>
#include<cmath>
class Vec3f;
class ForceField
{
public:
	virtual Vec3f getAcceleration(const Vec3f& position, float mass, float t) const = 0;
private:

};

class GravityForceField :public ForceField {
public:
	GravityForceField(Vec3f& gra):gravity(gra) {}
	Vec3f getAcceleration(const Vec3f& position, float mass, float t)const {
		return gravity;
	}
private:
	Vec3f gravity;
};

class ConstantForceField:public ForceField{
public:
	ConstantForceField(Vec3f foc):force(foc) {}
	Vec3f getAcceleration(const Vec3f& position, float mass, float t)const {
		assert(mass > 0);
		return force / mass;
	}
private:
	Vec3f force;
};
class RadialForceField:public ForceField {
public:
	RadialForceField(float mag) :magnitude(mag){}
	Vec3f getAcceleration(const Vec3f& position, float mass, float t)const {
		float d = position.Length();
		Vec3f force = position;
		force.Negate();
		force.Normalize();
		force *= (d * magnitude);
		return force / mass;
	}
private:
	float magnitude;
};

class VerticalForceField :public ForceField {
public:
	VerticalForceField(float mag) :magnitude(mag){}
	Vec3f getAcceleration(const Vec3f& position, float mass, float t)const {
		float d = fabsf(position.y());
		Vec3f force;
		force.Set(0., -position.y(), 0.);
		force.Normalize();
		force *= (d * magnitude);
		return force / mass;
	}
private:
	float magnitude;
};


#endif