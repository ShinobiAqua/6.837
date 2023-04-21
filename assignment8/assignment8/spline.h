#ifndef _SPLINE_H_
#define _SPLINE_H_
#include"matrix.h"
#include<assert.h>
#include<vector>
#include"arg_parser.h"
#include"triangle_mesh.h"
class TriangleMesh;
class Spline{
private:
	vector<Vec3f> ControlP;
public:
	Spline() {
		/*ControlP.push_back(Vec3f(0., 0., 0.));
		ControlP.push_back(Vec3f(0., 0., 1.));
		ControlP.push_back(Vec3f(0., 1., 0.));
		ControlP.push_back(Vec3f(0., 1., 1.));*/
	}
	void set(int i,Vec3f v) {
		while (i >= getNumVertices())
		{
			ControlP.push_back(Vec3f(0, 0, 0));
		}
		ControlP[i] = v;
	}
	// FOR VISUALIZATION
	//这个要是虚的 不同曲线绘制方法不一样
	virtual void Paint(ArgParser* args) {
	};

	
	
	// FOR CONVERTING BETWEEN SPLINE TYPES
	virtual void OutputBezier(FILE* file) {};
	virtual void OutputBSpline(FILE* file) {};

	// FOR CONTROL POINT PICKING
	virtual int getNumVertices() {
		return ControlP.size();
	}
	virtual Vec3f getVertex(int i) {
		assert(i < ControlP.size());
		return ControlP[i];
	}

	// FOR EDITING OPERATIONS
	virtual void moveControlPoint(int selectedPoint, float x, float y) {
		ControlP[selectedPoint].Set(x, y, 0.0);
	}
	//在id=pt处插入controlpoint
	virtual void addControlPoint(int selectedPoint, float x, float y) {
		ControlP.insert(ControlP.begin() + selectedPoint, Vec3f(x, y, 0.0));
	}
	virtual void deleteControlPoint(int selectedPoint) {
		if (ControlP.size() <= 4) return;
		ControlP.erase(ControlP.begin() + selectedPoint);
	}
	//virtual vector<Vec3f> getPoints(ArgParser* args) { vector<Vec3f>ans; return ans; };
	virtual void dePaint(ArgParser* args, int iter) {};
	// FOR GENERATING TRIANGLES
	virtual TriangleMesh* OutputTriangles(ArgParser* args) {
		TriangleMesh *p=new TriangleMesh(0,0);
		return p;
	};
};


#endif
