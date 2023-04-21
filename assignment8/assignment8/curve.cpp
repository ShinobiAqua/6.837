#include"curve.h"

static const float Bbezier[] = { -1.0 ,3.0 ,-3.0 ,1.0 ,3.0 ,-6.0 ,3.0 ,0.0 ,-3.0 ,3.0 ,0.0 ,0.0 ,1.0 ,0.0 ,0.0 ,0.0 };
Matrix BezierCurve::B(Bbezier);
static const float BBSpline[] = { -1.0/6,  0.5,  -0.5,  1.0/6,  0.5,  -1.0,  0.,  2.0/3,  -0.5,  0.5,  0.5,  1.0/6,  1.0/6,  0.,  0.,  0. };
Matrix BSplineCurve::B(BBSpline);



vector<Vec3f> BezierCurve::getPoints(ArgParser* args) {
	int itr = args->curve_tessellation;
	float cpr = 1.0 / itr;
	Vec4f ori;
	vector<Vec3f> ans;
	Vec3f tmp;
	float t;
	Vec3f cp[4];
	for (int i = 0; i < getNumVertices()-3; i+=3) {
		cp[0] = getVertex(i);
		cp[1] = getVertex(i + 1);
		cp[2] = getVertex(i + 2);
		cp[3] = getVertex(i + 3);
		for (int j = 0; j <= itr; j++) {
			t = j * cpr;
			ori.Set(t * t * t, t * t, t, 1.0);
			B.Transform(ori);
			tmp = cp[0] * ori[0] + cp[1] * ori[1] + cp[2] * ori[2] + cp[3] * ori[3];
			ans.push_back(tmp);
		}
	}
	return ans;
}
vector<Vec3f> BSplineCurve::getPoints(ArgParser* args) {
	int itr = args->curve_tessellation;
	float cpr = 1.0 / itr;
	vector<Vec3f> ans;
	Vec3f tmp;
	Vec3f cp[4];
	float t;
	Vec4f ori;
	for (int i = 0; i < getNumVertices() - 3; i++) {
		cp[0] = getVertex(i);
		cp[1] = getVertex(i + 1);
		cp[2] = getVertex(i + 2);
		cp[3] = getVertex(i + 3);
		for (int j = 0; j <= itr; j++) {
			t = j * cpr;
			ori.Set(t * t * t, t * t, t, 1.0);
			B.Transform(ori);
			tmp = cp[0] * ori[0] + cp[1] * ori[1] + cp[2] * ori[2] + cp[3] * ori[3];
			ans.push_back(tmp);
		}
	}
	return ans;
}
void Curve::Paint(ArgParser* args) {
	vector<Vec3f> poit = getPoints(args);
	Vec3f tmp;
	//画控制点
	glColor3f(255, 0, 0);
	glPointSize(10);
	glBegin(GL_POINTS);
	for (int k = 0; k < getNumVertices(); k++) {
		tmp = getVertex(k);
		glVertex2f(tmp[0], tmp[1]);
	}
	glEnd();

	//画控制点之间的边
	glColor3f(0, 0, 255);
	glLineWidth(2.0);
	glBegin(GL_LINES);
	for (int k = 0; k < getNumVertices(); k++) {
		tmp = getVertex(k);
		if (k == 0 || k == getNumVertices() - 1) {
			glVertex2f(tmp[0], tmp[1]);
		}
		else {
			glVertex2f(tmp[0], tmp[1]);
			glVertex2f(tmp[0], tmp[1]);
		}
	}
	glEnd();
	
	//画边
	glLineWidth(3.0);
	glColor3f(0., 255., 0.);
	glBegin(GL_LINES);
	for (int i = 0; i < poit.size(); i++)
	{
		if(i==0||i==(int)poit.size()-1)
			glVertex2f(poit[i].x(), poit[i].y());
		else {
			glVertex2f(poit[i].x(), poit[i].y());
			glVertex2f(poit[i].x(), poit[i].y());
		}
	}
	glEnd();
}


//di gui die dai
void BezierCurve::dePaint(ArgParser* args,int iter) {
	
	vector<Vec3f> ar;
	ar.push_back(getVertex(0));
	ar.push_back(getVertex(1));
	ar.push_back(getVertex(2));
	ar.push_back(getVertex(3));
	dsg(ar, 0, iter);
	glutPostRedisplay();
}
void BezierCurve::dsg(vector<Vec3f> ar, int depth, int iter) {

	if (depth >= iter) return;
	vector<Vec3f> left;
	vector<Vec3f> right;
	Vec3f p1 = (ar[0] + ar[1]);
	p1 /= 2;
	Vec3f p2 = (ar[1] + ar[2]);
	p2 /= 2;
	Vec3f p3 = (ar[2] + ar[3]);
	p3 /= 2;
	Vec3f p4 = (p1 + p2);
	p4 /= 2;
	Vec3f p5 = (p2 + p3);
	p5 /= 2;
	Vec3f p6 = (p4 + p5);
	p6 /= 2;
	left.push_back(ar[0]);
	left.push_back(p1);
	left.push_back(p4);
	left.push_back(p6);
	right.push_back(p6);
	right.push_back(p5);
	right.push_back(p3);
	right.push_back(ar[3]);
	if (1) {
		if (depth < 3) {
			glPointSize(5.0);
			glColor3f(255, 0, 0);
			glBegin(GL_POINTS);
			glVertex2f(p1.x(), p1.y());
			glVertex2f(p2.x(), p2.y());
			glVertex2f(p3.x(), p3.y());
			glVertex2f(p4.x(), p4.y());
			glVertex2f(p5.x(), p5.y());
			glEnd();
		}

		glLineWidth(0.5);
		glColor3f(0, 0, 255);
		glBegin(GL_LINES);
		glVertex2f(p1.x(), p1.y());
		glVertex2f(p2.x(), p2.y());
		glVertex2f(p2.x(), p2.y());
		glVertex2f(p3.x(), p3.y());
		glVertex2f(p4.x(), p4.y());
		glVertex2f(p5.x(), p5.y());
		glEnd();
	
	
		glLineWidth(1.0);
		glColor3f(0, 255, 0);
		glBegin(GL_LINES);
		glVertex2f(ar[0].x(), ar[0].y());
		glVertex2f(ar[1].x(), ar[1].y()); glVertex2f(ar[1].x(), ar[1].y());
		glVertex2f(ar[2].x(), ar[2].y()); glVertex2f(ar[2].x(), ar[2].y());
		glVertex2f(ar[3].x(), ar[3].y());
		glEnd();
	}
	dsg(left, depth + 1, iter);
	dsg(right, depth + 1, iter);
}


void BezierCurve::OutputBezier(FILE* file) {
	fprintf(file, "\nbezier\n");
	int t = getNumVertices();
	Vec3f tmp;
	fprintf(file, "num_vertices %d\n", t);
	for (int i = 0; i < t; i++) {
		tmp = getVertex(i);
		fprintf(file, "%.3f %.3f %.3f\n", tmp[0], tmp[1], tmp[2]);
	}
};
void BezierCurve::OutputBSpline(FILE* file) {
	Matrix Bsp;
	Bsp = BSplineCurve::getB();
	Matrix Bez = B;
	Matrix Trans;
	Bsp.Inverse();
	Trans = Bez * Bsp;
	/*Trans.print();
	Matrix tmmp = Trans;
	tmmp.Inverse();
	tmmp.print();*/
	fprintf(file, "\nbspline\n");
	fprintf(file, "num_vertices 4\n");
	Vec3f cp[4],tmp;
	cp[0] = getVertex(0);
	cp[1] = getVertex(1);
	cp[2] = getVertex(2);
	cp[3] = getVertex(3);
	for (int i = 0; i < 4; i++) {
		tmp = cp[0] * Trans.Get(i, 0) + cp[1] * Trans.Get(i, 1) +
			cp[2] * Trans.Get(i, 2) + cp[3] * Trans.Get(i, 3);
		fprintf(file, "%.3f %.3f %.3f\n", tmp[0], tmp[1], tmp[2]);
	}
};

void BSplineCurve::OutputBezier(FILE* file) {
	Matrix Bez;
	Bez = BezierCurve::getB();
	Matrix Bsp = B;
	Matrix Trans;
	Bez.Inverse();
	Trans = Bsp * Bez;
	fprintf(file, "\nbezier\n");
	fprintf(file, "num_vertices 4\n");

	Vec3f cp[4], tmp;
	//Trans.print();
	cp[0] = getVertex(0);
	cp[1] = getVertex(1);
	cp[2] = getVertex(2);
	cp[3] = getVertex(3);
	//Trans.print();
	for (int i = 0; i < 4; i++) {
		tmp = cp[0] * Trans.Get(i, 0) + cp[1] * Trans.Get(i, 1) +
			cp[2] * Trans.Get(i, 2) + cp[3] * Trans.Get(i, 3);
		//cout << cp[i]<<tmp << endl;
		fprintf(file, "%.3f %.3f %.3f\n", tmp[0], tmp[1], tmp[2]);
	}
};
void BSplineCurve::OutputBSpline(FILE* file) {
	fprintf(file, "\nbspline\n");
	int t = getNumVertices();
	Vec3f tmp;
	fprintf(file, "num_vertices %d\n", t);
	for (int i = 0; i < t; i++) {
		tmp = getVertex(i);
		fprintf(file, "%.3f %.3f %.3f\n", tmp[0], tmp[1], tmp[2]);
	}
};