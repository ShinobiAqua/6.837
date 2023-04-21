#ifndef _OBJECT3D_H
#define _OBJECT3D_H
#include <GL/glut.h>
#include <GL/gl.h>
#include <vector>
#include <assert.h>
#include"boundingbox.h"
#include"matrix.h"
class Grid;
class Ray;
class Hit;

class Object3D {
public:
	virtual bool intersect(const Ray& r, Hit& h, float tmin) = 0;
	Object3D() {
		box = NULL;
		isTri = false;
	}
	Object3D(bool ist) {
		box = NULL;
		isTri = ist;
	}
	virtual ~Object3D(){
		delete box;
	}
	virtual void paint(void) = 0;
	bool V_empty() {
		return vertices.empty();
	}
	void push(Vec3f a) {
		vertices.push_back(a);
	}
	Vec3f getV(int index) {
		assert(index < (int)vertices.size());
		return vertices[index];
	}
	virtual void insertIntoGrid(Grid* g, Matrix* m);
	BoundingBox* getBoundingbox() { return box; }
	void setBoundingBox(Vec3f _min,Vec3f _max) {
		//assert(box == NULL);
		box = new BoundingBox(_min, _max);
	}
	bool isTriangle() {
		return this->isTri;
	}
	virtual void flattern(vector<Object3D*>& arr, Matrix* m);
private:
	vector<Vec3f> vertices;
	BoundingBox* box;
	bool isTri;
};



#endif
