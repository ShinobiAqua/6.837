#ifndef  _CURVE_H_
#define _CURVE_H_
#include"spline.h"
#include"arg_parser.h"
#include<GL/glut.h>
#include<GL/gl.h>
#include<vector>
//class Spline;
class Curve:public Spline {
private:
public:
	void Paint(ArgParser* args);
	virtual vector<Vec3f> getPoints(ArgParser* args)=0;
};

class BezierCurve :public Curve {
private:
	static Matrix B;
public:
	BezierCurve() {}
	BezierCurve(int n) {}
	//void Paint(ArgParser* args);
	void dsg(vector<Vec3f> ar, int depth, int iter);
	void dePaint(ArgParser* args,int iter);
	vector<Vec3f> getPoints(ArgParser* args);
	void OutputBezier(FILE* file);
	void OutputBSpline(FILE* file);
	static Matrix getB()
	{
		return B;
	}
};

class BSplineCurve :public Curve {
private:
	static Matrix B;
public:
	BSplineCurve() {}
	BSplineCurve(int n) {}
	//void Paint(ArgParser* args);
	vector<Vec3f> getPoints(ArgParser* args);
	static Matrix getB()
	{
		return B;
	}
	void OutputBezier(FILE* file);
	void OutputBSpline(FILE* file);
};

#endif // ! _CURVE_H_

