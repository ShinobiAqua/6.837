#include "MarchingInfo.h"
#include"raytracing_stats.h"
#include<iostream>

using namespace std;
#define isMin(a,b,c) (((a)<(b))&&((a)<(c)))
void MarchingInfo::nextCell() {
	RayTracingStats::IncrementNumGridCellsTraversed();

	Vec3f xn;
	Vec3f yn;
	Vec3f zn;
	xn.Set(-sign_x, 0, 0);
	yn.Set(0, -sign_y, 0);
	zn.Set(0, 0, -sign_z);
	if (isMin(t_next_x, t_next_y, t_next_z)) {
		tmin = t_next_x;
		i = i + sign_x;
		t_next_x += dx;
		//cout << "x: " <<tmin<< endl;
		setNormal(xn);
	}
	else if (isMin(t_next_y, t_next_x, t_next_z)) {
		tmin = t_next_y;
		j += sign_y;
		t_next_y += dy;
		setNormal(yn);
		//cout << "y: " <<tmin<< endl;
	}
	else {
		tmin = t_next_z;
		k += sign_z;
		t_next_z += dz;
		setNormal(zn);
		//cout << "z: " <<tmin<< endl;
	}
}
void MarchingInfo::print() {
	cout << "infomation:\n";
	cout << tmin << endl;
	printf("i j k : %d  %d  %d\n", i, j, k);
	cout << "normal: "<<normal << endl;
	printf("deravitives: %f  %f  %f\n", dx, dy, dz);
	printf("sign: %d  %d  %d\n", sign_x, sign_y, sign_z);
	printf("t_next: %f  %f  %f\n", t_next_x, t_next_y, t_next_z);
	cout << "end of print\n";
}