#ifndef _OBJECT3D_H
#define _OBJECT3D_H
#include"ray.h"
#include"hit.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include <vector>
#include <assert.h>
class Object3D {
public:
	virtual bool intersect(const Ray& r, Hit& h, float tmin) = 0;
	virtual ~Object3D(){
		
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
private:
	vector<Vec3f> vertices;
};



#endif
