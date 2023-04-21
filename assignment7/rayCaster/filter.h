#ifndef _FILTER_H
#define _FILTER_H
#include "vectors.h"
#include"film.h"
#include"sample.h"

class Filter
{
public:
	virtual Vec3f getColor(int i, int j, Film* film);
	virtual float getWeight(float x, float y) = 0;
	virtual int getSupportRadius() = 0;
private:
};

class BoxFilter :public Filter {
public:
	BoxFilter(float x) :r(x) { assert(x >= 0); }
	int getSupportRadius() {
		return (int)ceil(r-0.5);
	}
	float getWeight(float x, float y) {
		if (x<-r || x>r || y<-r || y>r)return 0.0;
		return 1.0;
	}
private:
	float r;
};

class TentFilter :public Filter {
public:
	TentFilter(float x) :r(x) { assert(x >= 0); }
	int getSupportRadius() {
		return (int)ceil(r-0.5);
	}
	float getWeight(float x, float y) {
		float d = x * x + y * y;
		if (d > r* r) return 0.0;
		return 1.0 - sqrt(d)/r;
	}
private:
	float r;
};

class GaussianFilter :public Filter {
public:
	GaussianFilter(float sgm) :sigma(sgm) { assert(sgm >= 0); }
	int getSupportRadius() {
		return (int)ceil(2.0*sigma-0.5);
	}
	float getWeight(float x, float y) {
		float d = x * x + y * y;
		//if (d > 4*sigma*sigma) return 0.0;
		return expf(-d / (2 * sigma * sigma));
	}
private:
	float sigma;
};

#endif

