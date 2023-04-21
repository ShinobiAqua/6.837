#ifndef _GENERATOR_H
#define _GENERATOR_H
#define _USE_MATH_DEFINES
#include"random.h"
#include"vectors.h"
#include"particle.h"
#include<math.h>


class Generator {
public:
	Generator() {
		rander = new Random(0);
	}

	virtual ~Generator() {
		delete rander;
	}
	// initialization
	void SetColors(Vec3f color, Vec3f dead_color, float color_randomness) {
		this->color = color;
		this->dead_color = dead_color;
		this->color_randomness = color_randomness;
	}

	void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles) {
		this->lifespan = lifespan;
		this->lifespan_randomness = lifespan_randomness;
		this->desired_num_particles = desired_num_particles;
	}
	void SetMass(float mass, float mass_randomness) {
		this->mass = mass;
		this->mass_randomness = mass_randomness;
	}

	// on each timestep, create some particles
	virtual int numNewParticles(float current_time, float dt) const = 0;
	virtual Particle* Generate(float current_time, int i) = 0;

	// for the gui
	virtual void Paint() const {
	};
	void Restart() { delete rander; rander = new Random(0); }

	//============
	float getLifespan() const { return lifespan; }
	Random* getRander() const { return rander; }
	Vec3f getColor() const { return color; }
	Vec3f getDeadcolor() const { return dead_color; }
	float getLifespanRandomness()const { return lifespan_randomness; }
	int getDesiredNumParticles() const { return desired_num_particles; }
	float getMass() const { return mass; }
	float getMassRandomness() const { return mass_randomness; }
	float getColorRandomness() const { return color_randomness; }
private:
	Vec3f color, dead_color;
	int desired_num_particles;
	float 
		color_randomness,
		lifespan,
		lifespan_randomness,
		mass,
		mass_randomness;
	Random* rander;
};

class HoseGenerator :public Generator {
public:
	HoseGenerator(Vec3f position, float position_randomness,
		Vec3f velocity, float velocity_randomness) {
		this->position = position;
		this->velocity = velocity;
		this->position_randomness = position_randomness;
		this->velocity_randomness = velocity_randomness;
	}
	int numNewParticles(float current_time, float dt) const {
		return dt * getDesiredNumParticles() / getLifespan();
	}
	Particle* Generate(float current_time, int i);
private:
	Vec3f position,velocity;
	float position_randomness,
		velocity_randomness;

};

class RingGenerator :public Generator {
public:
	RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness) {
		this->velocity = velocity;
		this->position_randomness = position_randomness;
		this->velocity_randomness = velocity_randomness;
		max_time = 30.0;
		density = 5.0;
		expand_velocity = 1;
	}
	int numNewParticles(float current_time, float dt) const {
		if (current_time > max_time) return 0;
		float radius = current_time * expand_velocity;
		float C = 2 * M_PI * radius;
		int ans;
		ans = C * density;
		return ans;
	};
	Particle* Generate(float current_time, int i);
	virtual void Paint() const;
private:
	float max_time;
	//œﬂ√‹∂»
	float density;
	float expand_velocity;
	Vec3f velocity;
	float position_randomness,
		velocity_randomness;
};
#endif // !1


