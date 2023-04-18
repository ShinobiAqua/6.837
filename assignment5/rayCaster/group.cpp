#include"group.h"
#include<assert.h>
#include<iostream>
using namespace std;
extern bool griduse;
void Group::addObject(int index,Object3D *obj) {
	assert(index < num_objects);
	primitives[index] = obj;
	if (griduse) {
		BoundingBox* box = getBoundingbox();
		BoundingBox* tmp = primitives[index]->getBoundingbox();
		if (!tmp) return;
		if (box == NULL) {
			setBoundingBox(tmp->getMin(), tmp->getMax());
			box = new BoundingBox(tmp->getMin(), tmp->getMax());
		}
		else {
			box->Extend(tmp);
		}
	}
}
bool Group::intersect(const Ray& r, Hit& h, float tmin) {
	bool isinter = false;
	for (int i = 0; i < num_objects; i++) {
		isinter|=primitives[i]->intersect(r, h, tmin);
	}
	return isinter;
 }
void Group::paint()
{
	for (int i = 0; i < num_objects; i++) {
		primitives[i]->paint();
	}
}