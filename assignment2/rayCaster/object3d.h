#ifndef _OBJECT3D_H
#define _OBJECT3D_H
#include"ray.h"
#include"hit.h"
#include <GL/glut.h>
#include <GL/gl.h>
class Object3D {
public:
	virtual bool intersect(const Ray& r, Hit& h, float tmin) = 0;
	virtual ~Object3D(){
		
	}
	virtual void paint(void){
	};
private:
};



#endif
