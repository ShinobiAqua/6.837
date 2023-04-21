#ifndef _SAMPLER_H
#define _SAMPLER_H
#include"vectors.h"
#include<assert.h>
#include<cmath>
#include<iostream>
using namespace std;

class Vec2f;
class Sampler
{
public:
	virtual Vec2f getSamplePosition(int x) = 0;
private:
};

class RandomSampler :public Sampler {
public:
	Vec2f getSamplePosition(int x) {
		return Vec2f(rand() % 256 / 256.0, rand() % 256 / 256.0);
	}
private:
};
class UniformSampler :public Sampler {

public:
	UniformSampler(int x){
		assert(x >= 0);
		n = (int)sqrt(x);
	}
	Vec2f getSamplePosition(int x) {
		int i, j;
		i = (x / n) % n;
		j = x % n;
		Vec2f ans((0.5 + i), (0.5 + j));
		ans *= (1.0f / n);
		//cout << x << ' ' << ans.x()<<' '<<ans.y() << endl;
		return ans;
	}
private:
	int n;
};

class JitteredSampler :public Sampler {
public:
	JitteredSampler(int x) {
		assert(x >= 0);
		n = (int)sqrt(x);
	}
	Vec2f getSamplePosition(int x) {
		int i, j;
		i = (x / n) % n;
		j = x % n;
		Vec2f ans((0.5 + i), (0.5 + j));
		ans *= (1.0f / n);
		Vec2f rd(rand() % 256 / 256.0, rand() % 256 / 256.0);
		rd -= Vec2f(0.5, 0.5);
		rd *= (1.0f / n);
		ans += rd;
		return ans;
	}
private:
	int n;
};


#endif

