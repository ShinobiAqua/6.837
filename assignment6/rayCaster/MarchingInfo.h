#ifndef  _MARCHINGINFO_H
#define  _MARCHINGINFO_H
#include"vectors.h"


class MarchingInfo
{
public:
	MarchingInfo() {
	};
	MarchingInfo(MarchingInfo& mcf) {
		tmin = mcf.tmin;
		i = mcf.i; j = mcf.j; k = mcf.k;
		dx = mcf.dx; dy = mcf.dy; dz = mcf.dz;
		t_next_x = mcf.t_next_x;
		t_next_y = mcf.t_next_y;
		t_next_z = mcf.t_next_z;
		sign_x = mcf.sign_x;
		sign_y = mcf.sign_y;
		sign_z = mcf.sign_z;
		normal = mcf.normal;
	}
	MarchingInfo& operator=(MarchingInfo &mcf) {
		tmin = mcf.tmin;
		i = mcf.i; j = mcf.j; k = mcf.k;
		dx = mcf.dx; dy = mcf.dy; dz = mcf.dz;
		t_next_x = mcf.t_next_x;
		t_next_y = mcf.t_next_y;
		t_next_z = mcf.t_next_z;
		sign_x = mcf.sign_x;
		sign_y = mcf.sign_y;
		sign_z = mcf.sign_z;
		normal = mcf.normal;
		return *this;
	}
	void setSign(int x, int y, int z) {
		sign_x = x;
		sign_y = y;
		sign_z = z;
	}
	void setcell(int x, int y, int z) {
		i = x; j = y; k = z;
	}
	void setDerivative(float x,float y,float z) {
		dx = x; dy = y; dz = z;
	}
	void setTmin(float t) {
		tmin = t;
	}
	void setNext(float tx,float ty,float tz) {
		t_next_x = tx ;
		t_next_y = ty ;
		t_next_z = tz ;
		//=======================================

		if (t_next_x == -FLT_MAX)
			t_next_x = -t_next_x;
		else 
			while (t_next_x <= tmin) t_next_x += dx;

		//=======================================

		if (t_next_y == -FLT_MAX)
			t_next_y = -t_next_y;
		else
			while (t_next_y <= tmin) t_next_y += dy;

		//=======================================

		if (t_next_z == -FLT_MAX)
			t_next_z = -t_next_z;
		else
			while (t_next_z <= tmin) t_next_z += dz;

		//=======================================
	}
	void nextCell();
	void setNormal(Vec3f &n) {
		normal = n;
	}
	Vec3f getNormal() {
		return normal;
	}
	void getCube(int& x, int& y, int& z) {
		x = i; y = j; z = k;
	}
	float getTmin() {
		return tmin;
	}
	void print();
private:
	float tmin;
	int i, j, k;
	float dx, dy, dz;
	float t_next_x, t_next_y, t_next_z;
	int sign_x, sign_y, sign_z;
	Vec3f normal;
};

#endif // ! _MARCHINGINFO_H