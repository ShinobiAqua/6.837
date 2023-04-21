#ifndef _GROUP_H
#define _GROUP_H
#include"object3d.h"
#include<vector>

class Group :public Object3D {
public:
	Group(int num):num_objects(num) {
		primitives = new Object3D * [num];
	}
	~Group() {
		delete[] primitives;
	}
	void addObject(int index,Object3D *obj);
	virtual bool intersect(const Ray& r, Hit& h, float tmin);
	void paint();
	virtual void insertIntoGrid(Grid* g, Matrix* m) {
		for (int i = 0; i < num_objects; i++) {
			primitives[i]->insertIntoGrid(g, m);
		}
	}
	
private:
	int num_objects;
	Object3D** primitives;
};
#endif
