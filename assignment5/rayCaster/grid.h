#ifndef _GRID_H
#define _GRID_H
#include<iostream>
#include<cmath>
#include<algorithm>
#include<vector>
#include<assert.h>
#include"boundingbox.h"
#include"MarchingInfo.h"
#include"ray.h"
#include"rayTree.h"
#include"object3d.h"

using namespace std;


class Object3D;

class Grid:public Object3D{
public:
	Grid(BoundingBox* bb, int xi, int yi, int zi,RayTree *rt):raytree(rt),box(bb),nx(xi)
	,ny(yi),nz(zi){
		Mt = new Material * [10];
		//cout << "ssb1" << endl;
		for (int i = 0; i < 10; i++) {
			Vec3f color( 0.5,1.0 - 0.2 * i ,0.5);
			Mt[i] = new PhongMaterial(color);
		}
		//cout << "ssb2" << endl;
		voxel = new bool** [nx];
		for (int i = 0; i < nx; i++) {
			voxel[i] = new bool* [ny];
			for (int j = 0; j < ny; j++) {
				voxel[i][j] = new bool[nz];
				for (int k = 0; k < nz; k++)
					//false 表示透明
					voxel[i][j][k] = false;
			}
		}
		//cout << "ssb3" << endl;
		obj = new vector<Object3D*> **[nx];
		for (int i = 0; i < nx; i++) {
			obj[i] = new vector<Object3D*> *[ny];
			for (int j = 0; j < ny; j++) {
				obj[i][j] = new  vector<Object3D*> [ny];
			}
		}
		//cout << "ssb4" << endl;
		

		//cout << box << endl;
		Vec3f orig = box->getMin();
		Vec3f mxx = box->getMax();
		lx = mxx.x() - orig.x();
		ly = mxx.y() - orig.y();
		lz = mxx.z() - orig.z();
		//cout << orig << mxx << endl;
	
	}
	~Grid() {
		for (int i = 0; i < nx; i++) {
			for (int j = 0; j < ny; j++) {
				voxel[i][j] = new bool[nz];
				delete[] voxel[i][j];
				delete[] obj[i][j];
			}
			delete[] voxel[i];
			delete[] obj[i];
		}
		delete[] voxel;
		delete[] obj;
	}
	bool getVoxel(int i, int j, int k){
		if (i < 0 || j < 0 || k < 0) return false;
		if (i >= nx || j >= ny || k >= nz) return false;
		return voxel[i][j][k];
	}
	void setObj(int i,int j,int k,Object3D *ob) {
		obj[i][j][k].push_back(ob);
	}
	Object3D* getObj(int i, int j, int k, int t) {
		assert(t < (int)obj[i][j][k].size());
		return obj[i][j][k][t];
	}
	int getObjNum(int i, int j, int k) {
		return (int)obj[i][j][k].size();
	}
	void setVoxel(int i, int j, int k,bool st) {
		voxel[i][j][k] = st;
	}
	//格点编号0，0，0是方块编号0，0，0 的左下角
	Vec3f getPoint(int i, int j, int k) {
		Vec3f ans(0, 0, 0);
		Vec3f orig = box->getMin();
		Vec3f ax(lx, 0, 0);
		Vec3f ay(0, ly, 0);
		Vec3f az(0, 0, lz);
		ans = orig + i / float(nx) * ax + j / float(ny) * ay + k / float(nz) * az;
		return ans;
	}
	int x() {
		return nx;
	}
	int y() {
		return ny;
	}
	int z() {
		return nz;
	}
	//获得point所在的cell坐标
	Vec3f getCubeLow(Vec3f point) {
		float cx, cy, cz;
		Vec3f orig = box->getMin();
		cx = (int)((point.x()-orig.x()) / lx * nx);
		cy = (int)((point.y()-orig.y()) / ly * ny);
		cz = (int)((point.z()-orig.z()) / lz * nz);
		Vec3f ans(cx, cy, cz);
		ans.Clamp(Vec3f(0, 0, 0), Vec3f(nx-1, ny-1, nz-1));
		return ans;
	}
	//获得point所在的右上角的cell坐标
	Vec3f getCubeHigh(Vec3f point) {
		float cx, cy, cz;
		Vec3f orig = box->getMin();
		cx = (int)((point.x() - orig.x()) / lx * nx+1);
		cy = (int)((point.y() - orig.y()) / ly * ny+1);
		cz = (int)((point.z() - orig.z()) / lz * nz+1);
		Vec3f ans(cx, cy, cz);
		ans.Clamp(Vec3f(0, 0, 0), Vec3f(nx-1, ny-1, nz-1));
		return ans;
	}
	void print()
	{
		cout << "Print_Start\n";
		cout << nx << ' ' << ny << ' ' << nz << endl;
		for (int i = 0; i < nx; i++) {
			for (int j = 0; j < ny; j++) {
				for (int k = 0; k < nz; k++) {
					if (voxel[i][j][k])
						cout << 1 << ' ';
					else
						cout << 0 << ' ';
				}
				cout << endl;
			}
			cout << "------------------------\n";
		}
		cout << "Print_Done\n";
	}
	void paint();
	BoundingBox*  getBoundingBox() {
		return box;
	}
	bool intersect(const Ray& r, Hit& h, float tmin);
	bool initializeRayMarch(MarchingInfo& mi, const Ray& r, float tmin);
	//return (tmin_x,tmin_y,tmin_z)
	bool intersectWithCube(const Ray& r, Vec3f _min, Vec3f _max,
		Vec3f &tmin,Vec3f &tmax, float& tnear, float& tfar,float tm);
private:
	BoundingBox* box;
	float lx, ly, lz;
	int nx, ny, nz;
	bool*** voxel;
	vector<Object3D*>*** obj;
	RayTree* raytree;
	Material **Mt;
};
#endif

