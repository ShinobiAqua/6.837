#define _USE_MATH_DEFINES
#include <GL/glut.h>
#include <GL/gl.h>
#include "generator.h"
#include"random.h"
#include"vectors.h"
#include"particle.h"
#include<math.h>
#define GETaxis(v) (v).x(),(v).y(),(v).z()
const int BIG = 80000;
Particle* HoseGenerator::Generate(float current_time, int i) {
	Random* rander = getRander();
	Vec3f p, v, c;
	float m;
	float ls;
	Vec3f r1, r2, r3;
	float r4, r5;
	r1 = rander->randomVector() * position_randomness;
	p = position + r1;
	r2 = rander->randomVector() * velocity_randomness;
	v = velocity + r2;
	r3 = rander->randomVector() * getColorRandomness();
	c = getColor() + r3;
	r4 = rander->next() * getMassRandomness();
	m = getMass() + r4;
	r5 = rander->next() * getLifespanRandomness();
	ls = getLifespan() + r5;
	Particle* ans = new Particle(p, v, c, getDeadcolor(), m, ls);
	return ans;
};

Particle* RingGenerator::Generate(float current_time, int i) {
	Random* rander = getRander();
	float radius = current_time * expand_velocity;
	float angle = 2 * M_PI * rander->next();
	float x, z;
	x = radius * cos(angle);
	z = radius * sin(angle);
	Vec3f p(x, 0, z);
	Vec3f v, c;
	float m;
	float ls;
	Vec3f r1, r2, r3;
	float r4, r5;
	r1 = rander->randomVector() * position_randomness;
	p += r1;
	r2 = rander->randomVector() * velocity_randomness;
	v = velocity + r2;
	r3 = rander->randomVector() * getColorRandomness();
	c = getColor() + r3;
	r4 = rander->next() * getMassRandomness();
	m = getMass() + r4;
	r5 = rander->next() * getLifespanRandomness();
	ls = getLifespan() + r5;
	Particle* ans = new Particle(p, v, c, getDeadcolor(), m, ls);
	return ans;
};

void RingGenerator::Paint()const {
	Vec3f b2(1, 0, 0);
	Vec3f b1(0, 0, 1);
	Vec3f tmp(0, -4, 0);
	Vec3f v1, v2, v3, v4;
	v1 = BIG * b1 + BIG * b2;
	v2 = (-BIG) * b1 + BIG * b2;
	v3 = (-BIG) * b1 + (-BIG) * b2;
	v4 = BIG * b1 + (-BIG) * b2;
	v1 += tmp;
	v2 += tmp;
	v3 += tmp;
	v4 += tmp;
	glColor3f(0.4, 0.4, 0.4);
	glBegin(GL_QUADS);
	glVertex3f(GETaxis(v1));
	glVertex3f(GETaxis(v2));
	glVertex3f(GETaxis(v3));
	glVertex3f(GETaxis(v4));
	glEnd();

}