#include"boundingbox.h"
#include <GL/glut.h>
#include <GL/gl.h>
#define GETaix(v) v.x(),v.y(),v.z()
void BoundingBox::paint()const {
	glLineWidth(1);
	glColor3f(1.0, 1.0, 1.0);
	Vec3f v[8];
	float a[6];
	min.Get(a[0], a[2], a[4]);
	max.Get(a[1], a[3], a[5]);
	//底部顺时针 v0 v2 v6 v4
	//顶部顺时针 v1 v3 v7 v5
	int ct = 0;
	for(int i=0;i<2;i++)
		for(int j=2;j<4;j++)
			for (int k = 4; k < 6; k++) {
				v[ct++].Set(a[i], a[j], a[k]);
			}
	glBegin(GL_LINES);
	glVertex3f(GETaix(v[0]));
	glVertex3f(GETaix(v[2]));
	glVertex3f(GETaix(v[2]));
	glVertex3f(GETaix(v[6]));
	glVertex3f(GETaix(v[6]));
	glVertex3f(GETaix(v[4]));
	glVertex3f(GETaix(v[4]));
	glVertex3f(GETaix(v[0]));
	glVertex3f(GETaix(v[1]));
	glVertex3f(GETaix(v[5]));
	glVertex3f(GETaix(v[5]));
	glVertex3f(GETaix(v[7]));
	glVertex3f(GETaix(v[7]));
	glVertex3f(GETaix(v[3]));
	glVertex3f(GETaix(v[3]));
	glVertex3f(GETaix(v[1]));
	glVertex3f(GETaix(v[0]));
	glVertex3f(GETaix(v[1]));
	glVertex3f(GETaix(v[2]));
	glVertex3f(GETaix(v[3]));
	glVertex3f(GETaix(v[4]));
	glVertex3f(GETaix(v[5]));
	glVertex3f(GETaix(v[6]));
	glVertex3f(GETaix(v[7]));
	glEnd();
}