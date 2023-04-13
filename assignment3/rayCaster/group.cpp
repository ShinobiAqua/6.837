#include"group.h"
#include<assert.h>
void Group::addObject(int index,Object3D *obj) {
	assert(index < num_objects);
	primitives[index] = obj;
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