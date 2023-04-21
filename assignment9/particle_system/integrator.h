#ifndef _INTEGRATOR_H
#define _INTEGRATOR_H
#include"vectors.h"
#include"particle.h"
#include"forceField.h"
class Integrator {
public:
	virtual void Update(Particle* particle, ForceField* forcefield, float t, float dt) = 0;
	virtual Vec3f getColor() = 0;
private:
};


class EulerIntegrator:public Integrator{
public:
	void Update(Particle* particle, ForceField* forcefield, float t, float dt) {
		//cout << 111 << endl;
		Vec3f position = particle->getPosition();
		Vec3f velocity = particle->getVelocity();
		float mass = particle->getMass();
		Vec3f acceleration = forcefield->getAcceleration(position, mass, t);
		Vec3f new_position, new_velocity;
		position += velocity * dt;
		velocity += acceleration * dt;
		particle->setPosition(position);
		particle->setVelocity(velocity);
		particle->increaseAge(dt);
		//particle->print();
	}
	Vec3f getColor() { return Vec3f(1.0, 0, 0); }
private:
};



class MidpointIntegrator:public Integrator {
public:
	void Update(Particle* particle, ForceField* forcefield, float t, float dt) {
		Vec3f position = particle->getPosition();
		Vec3f velocity = particle->getVelocity();
		float mass = particle->getMass();
		
		//mid state
		Vec3f mid_position = position + dt * velocity / 2.0;
		Vec3f acceleration = forcefield->getAcceleration(mid_position, mass, t );
		Vec3f mid_velocity = velocity + acceleration * dt / 2.0;
		Vec3f mid_acceleration = forcefield->getAcceleration(mid_position, mass, t + dt/2.0);

		
		//final state
		position += mid_velocity * dt;
		velocity += mid_acceleration * dt;
		particle->setPosition(position);
		particle->setVelocity(velocity);
		particle->increaseAge(dt);
	}
	Vec3f getColor() { return Vec3f(0, 1.0, 0); }
private:
};


#endif // !1
