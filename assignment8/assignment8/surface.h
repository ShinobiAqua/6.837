#define _USE_MATH_DEFINES 1
#ifndef _SURFACE_H_
#define _SURFACE_H_
#include"triangle_mesh.h"
#include"arg_parser.h"
#include"curve.h"
#include<math.h>
#include<vector>
class Spline;
class Curve;
class Surface :public Spline {
private:
public:
	void Paint(ArgParser* args) {
	}
};

class SurfaceOfRevolution :public Surface {
private:
	Curve* c;
public:
	SurfaceOfRevolution(Curve* c) {
		this->c = c;
	}
	~SurfaceOfRevolution() {
		delete c;
	}
	TriangleMesh* OutputTriangles(ArgParser* args) {
		vector<Vec3f> gt;
		gt = c->getPoints(args);
		int cuvt = (int)gt.size()-1;
		int angt = args->revolution_tessellation;
		TriangleMesh* ans = new TriangleMesh((cuvt+1) * (angt + 1), (cuvt) * 2 * angt);
		float angle = 2 * M_PI;
		Matrix Rot;
		Rot = Matrix::MakeYRotation(2.0 * M_PI / angt);
		//Rot.print();
		int ct = 0;
		for (int i = 0; i <= angt; i++) {
			for (int j = 0; j < cuvt+1; j++) {
				ans->SetVertex(ct++,gt[j]);
				Rot.Transform(gt[j]);
			}
		}
	
		//加上三角形
		ct = 0;
		int t1,t2,t3,t4;
		for (int i = 0; i < angt; i++) {
			for (int j = 0; j < cuvt; j++) {
				t1 = i * (cuvt + 1) + j;
				t2 = t1 + 1;
				t3 = t1 + cuvt + 1;
				t4 = t3 + 1;
				ans->SetTriangle(ct++, t3, t2, t4);
				ans->SetTriangle(ct++, t1, t2, t3);
				/*printf("%d %d %d\n", t3, t2, t4);
				printf("%d %d %d\n", t1, t2, t3);*/
				
			}
		}
		return ans;
	}
	void Paint(ArgParser* args) {
		c->Paint(args);
	}
	int getNumVertices() {
		return c->getNumVertices();
	}
	Vec3f getVertex(int i) {
		return c->getVertex(i);
	}

	// FOR EDITING OPERATIONS
	virtual void moveControlPoint(int selectedPoint, float x, float y) {
		c->moveControlPoint(selectedPoint, x, y);
	}
	//在id=pt处插入controlpoint
	virtual void addControlPoint(int selectedPoint, float x, float y) {
		c->addControlPoint(selectedPoint, x, y);
	}
	virtual void deleteControlPoint(int selectedPoint) {
		c->deleteControlPoint(selectedPoint);
	}
	void OutputBSpline(FILE* file) {
		fprintf(file, "\nsurface_of_revolution");
		c->OutputBSpline(file);
	}
	void OutPutBezier(FILE* file) {
		fprintf(file, "\nsurface_of_revolution");
		c->OutputBezier(file);
	}
};
class BezierPatch :public Surface {
private:
	Matrix B;
public:
	BezierPatch() {
		B = BezierCurve::getB();
	}
	Vec3f getPoint(float t, float s) {
		Vec3f cp[16];
		Vec3f cp2[4];
		Vec4f ori;
		Vec3f ans;
		ori.Set(t * t * t, t * t, t, 1.0);
		B.Transform(ori);
		for (int i = 0; i < 16; i++)
			cp[i] = getVertex(i);
		for (int i = 0; i < 4; i++) {
			cp2[i] = cp[i * 4 + 0] * ori[0] + cp[i * 4 + 1] * ori[1] 
				+ cp[i * 4 + 2] * ori[2] + cp[i * 4 + 3] * ori[3];
		}
		ori.Set(s * s * s, s * s, s, 1.0);
		B.Transform(ori);
		ans = cp2[0] * ori[0] + cp2[1] * ori[1]
			+ cp2[2] * ori[2] + cp2[3] * ori[3];
		return ans;
	}
	TriangleMesh* OutputTriangles(ArgParser* args) {
		int tess = args->patch_tessellation;
		//v_num=(tess+1)^2
		//f_num=2*tess*tess
		TriangleNet* ans = new TriangleNet(tess,tess);
		Vec3f v;
		float t, s;
		float tp, sp;
		tp = sp = 1.0 / tess;
		for (int i = 0; i <= tess; i++) {
			for (int j = 0; j <= tess; j++) {
				t = i * tp;
				s = j * sp;
				v = getPoint(t, s);
				ans->SetVertex(i, j, v);
			}
		}
		return ans;
	}
	void Paint(ArgParser* args) {
		Vec3f tmp;
		glColor3f(255, 0, 0);
		glPointSize(10);
		glBegin(GL_POINTS);
		for (int k = 0; k < getNumVertices(); k++) {
			tmp = getVertex(k);
			glVertex2f(tmp[0], tmp[1]);
		}
		glEnd();

		int tess = args->patch_tessellation;
		int numt = tess * tess * 2;
		TriangleNet* ans = new TriangleNet(tess, tess);
		Vec3f v;
		float t, s;
		float tp, sp;
		tp = sp = 1.0 / tess;
		for (int i = 0; i <= tess; i++) {
			for (int j = 0; j <= tess; j++) {
				t = i * tp;
				s = j * sp;
				v = getPoint(t, s);
				ans->SetVertex(i, j, v);
				//cout << v << endl;
			}
		}


		
		vector<Vec3f> tvs;
		glColor3f(0, 255, 0);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < numt; i++) {
			tvs = ans->getTriangle(i);
		glVertex3f(tvs[0][0], tvs[0][1], tvs[0][2]);
		glVertex3f(tvs[1][0], tvs[1][1], tvs[1][2]);
		glVertex3f(tvs[2][0], tvs[2][1], tvs[2][2]);
		}
		//cout << tvs[0] << tvs[1] << tvs[2] << endl;
		
		glEnd();
		


		delete ans;
	}
};
#endif // !_SURFACE_H_
