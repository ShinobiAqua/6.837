#include "grid.h"
#include <GL/glut.h>
#include <GL/gl.h>
#include "raytracing_stats.h"
#include<unordered_map>
#define GETaix(v) (v).x(),(v).y(),(v).z()
#define GETaix_add1(v) (v).x()+1,(v).y()+1,(v).z()+1
#define parel 1e20
bool stopquick = true;
const float epsilon = 1e-8;
void Grid::paint()
{
	for (int i = 0; i < nx; i++) {
		for (int j = 0; j < ny; j++) {
			for (int k = 0; k < nz; k++) {
				if (getVoxel(i, j, k)) {
					Vec3f v1, v2, v3, v4,v5,v6,v7,v8,norm;
					//8个点从底向上 顺时针
					v1 = getPoint(i, j, k);
					v2 = getPoint(i, j + 1, k);
					v3 = getPoint(i + 1, j + 1, k);
					v4 = getPoint(i + 1, j, k);
					v5 = getPoint(i, j, k+1);
					v6 = getPoint(i, j + 1, k+1);
					v7 = getPoint(i + 1, j + 1, k+1);
					v8 = getPoint(i + 1, j, k+1);
					int ovlp = getObjNum(i,j,k)%10;
					Mt[ovlp]->glSetMaterial();
					if (!getVoxel(i - 1, j, k)) {
						/*Vec3f tmp = v1 + Vec3f(-1, 0, 0);
						glBegin(GL_LINES);
						glVertex3f(GETaix(v1));
						glVertex3f(GETaix(tmp));
						glEnd();*/

						glBegin(GL_QUADS);
						glNormal3f(-1,0,0);
						glVertex3f(GETaix(v1));glVertex3f(GETaix(v5));glVertex3f(GETaix(v6));
						glVertex3f(GETaix(v2));
						
						
						glEnd();
					}
					if (!getVoxel(i + 1, j, k)) {
						/*Vec3f tmp = v1 + Vec3f(1, 0, 0);
						glBegin(GL_LINES);
						glVertex3f(GETaix(v1));
						glVertex3f(GETaix(tmp));
						glEnd();*/

						glBegin(GL_QUADS);
						glNormal3f(1, 0, 0);
						glVertex3f(GETaix(v4));glVertex3f(GETaix(v3));glVertex3f(GETaix(v7));
						glVertex3f(GETaix(v8));
						
						
						glEnd();
					}
					if (!getVoxel(i , j-1, k)) {
						/*Vec3f tmp = v1 + Vec3f(0, -1, 0);
						glBegin(GL_LINES);
						glVertex3f(GETaix(v1));
						glVertex3f(GETaix(tmp));
						glEnd();*/

						glBegin(GL_QUADS);
						glNormal3f(0, -1, 0);
						glVertex3f(GETaix(v1));glVertex3f(GETaix(v4));glVertex3f(GETaix(v8));
						glVertex3f(GETaix(v5));
						
						
						glEnd();
					}
					if (!getVoxel(i , j+1, k)) {
						/*Vec3f tmp = v1 + Vec3f(0, 1, 0);
						glBegin(GL_LINES);
						glVertex3f(GETaix(v1));
						glVertex3f(GETaix(tmp));
						glEnd();*/

						glBegin(GL_QUADS);
						glNormal3f(0, 1, 0);
						glVertex3f(GETaix(v2));glVertex3f(GETaix(v6));glVertex3f(GETaix(v7));
						glVertex3f(GETaix(v3));
						
						
						glEnd();
					}
					if (!getVoxel(i , j, k-1)) {
						/*Vec3f tmp = v1 + Vec3f(0, 0, -1);
						glBegin(GL_LINES);
						glVertex3f(GETaix(v1));
						glVertex3f(GETaix(tmp));
						glEnd();*/

						glBegin(GL_QUADS);
						glNormal3f(0, 0, -1);
						glVertex3f(GETaix(v1));
						glVertex3f(GETaix(v2));
						glVertex3f(GETaix(v3));
						glVertex3f(GETaix(v4));
						glEnd();
					}
					if (!getVoxel(i , j, k+1)) {
						/*Vec3f tmp = v1 + Vec3f(0, 0, 1);
						glBegin(GL_LINES);
						glVertex3f(GETaix(v1));
						glVertex3f(GETaix(tmp));
						glEnd();*/

						glBegin(GL_QUADS);
						glNormal3f(0, 0, 1);
						glVertex3f(GETaix(v5));glVertex3f(GETaix(v8));glVertex3f(GETaix(v7));
						glVertex3f(GETaix(v6));
						
						
						glEnd();
					}

				}
			}
		}
	}
}


bool Grid::initializeRayMarch(MarchingInfo& mi, const Ray& r, float tmin){
	Vec3f orig = r.getOrigin();
	Vec3f _min = box->getMin();
	Vec3f _max = box->getMax();
	Vec3f cube;
	Vec3f dir = r.getDirection();
	//dir.Normalize();
	//cout << "dir"<<dir << endl;
	int sx, sy, sz;
	Vec3f vc = box->getMax() - box->getMin();
	vc.Normalize();

	//前进方向
	sx = dir.x()*vc.x() > 0 ? 1 : -1;
	sy = dir.y()*vc.y() > 0 ? 1 : -1;
	sz = dir.z()*vc.z() > 0 ? 1 : -1;
	mi.setSign(sx, sy, sz);

	// r.dir has to be normalized
	//marching距离
	float dx, dy, dz;
	//每个cell的x相差lx/nx
	dx = dir.x() != 0 ? (lx / (nx*dir.x())):0;
	dy = dir.y() != 0 ? (ly / (ny*dir.y())):0;
	dz = dir.z() != 0 ? (lz / (nz*dir.z())):0;
	//由于用于增大t，因此恒正
	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	if (dz < 0) dz = -dz;
	mi.setDerivative(dx, dy, dz);
	Vec3f intercell,_tmin,_tmax;
	float tnear,tfar;
	//射入的cell的表面方向总是和光线方向相反
	Vec3f xn(-sx, 0, 0);
	Vec3f yn(0, -sy, 0);
	Vec3f zn(0, 0, -sz);
	
	//光线原点在box内部
	if (orig > _min&& orig < _max) {
		intercell = getCubeLow(orig);
		mi.setcell(GETaix(intercell));
		Vec3f cmin, cmax;
		cmin = getPoint(GETaix(intercell));
		cmax = getPoint(GETaix_add1(intercell));
		intersectWithCube(r, cmin, cmax,_tmin,_tmax, tnear, tfar,tmin);
		mi.setTmin(tnear);
		if (tnear == _tmin.x()) 
				mi.setNormal(xn);
		
		else if (tnear == _tmin.y()) 
				mi.setNormal(yn);
		
		else 
				mi.setNormal(zn);

		/*cout << 6666 << endl;
		mi.print();*/
			
	
	}
	else {
		//光线原点在box外部
		bool isinter = intersectWithCube(r, _min, _max,_tmin,_tmax, tnear, tfar,tmin);
		if (!isinter)
		{
			//cout << 1 << endl;
			return false;
		}
		//cout << tnear << " " << tfar << endl;
		//cout << "intercell" << intercell << endl;
		intercell = r.pointAtParameter(tnear + epsilon);
		//cout << "intercell"<<intercell << endl;
		intercell = getCubeLow(intercell);

		//cout << "intercell" << intercell << endl;
		mi.setcell(GETaix(intercell));
		mi.setTmin(tnear);
		

		if (tnear == _tmin.x())
				mi.setNormal(xn);
		else if (tnear == _tmin.y()) 
				mi.setNormal(yn);
		else
				mi.setNormal(zn);
	
		//mi.setNext(GETaix())
	}
	mi.setNext(GETaix(_tmin));
	//mi.print();
	//cout << "_min,_max" << _min << ' ' << _max << endl;
	return true;
}


bool Grid::intersectWithCube(const Ray& r, Vec3f _min, Vec3f _max,
	Vec3f &tmin, Vec3f &tmax, float& tnear, float& tfar,
	float tm) {
	RayTracingStats::IncrementNumIntersections();
	Vec3f R0 = r.getOrigin();
	Vec3f Rd = r.getDirection();
	//Rd.Normalize();
	Vec3f t1, t2;
	t1 = (_min - R0) / Rd;
	t2 = (_max - R0) / Rd;
	
	/*cout << "min max" << _min << "  " << _max << endl;
	cout << t1 << "  " << t2 << endl;
	cout << Rd << endl;
	cout << R0 << endl;*/
	float d1, d2, d3;
	t1.Get(d1, d2, d3);
	if (d1 == FLT_MAX) d1 = -d1;
	if (d2 == FLT_MAX) d2 = -d2;
	if (d3 == FLT_MAX) d3 = -d3;
	t1.Set(d1, d2, d3);

	Vec3f::Min(tmin, t1, t2);
	Vec3f::Max(tmax, t1, t2);

	//cout << tmin << tmax << endl;

	tnear = max(max(tmin.x(),tmin.y()),tmin.z());
	tfar = min(min(tmax.x(),tmax.y()),tmax.z());

	//cout << tnear <<' '<< tfar << endl;
	/*cout << "tmp\n";
	Vec3f tmp = r.pointAtParameter(t1.x());
	cout << tmp << endl;
	tmp = r.pointAtParameter(t1.y());
	cout << tmp << endl;
	 tmp = r.pointAtParameter(t1.z());
	cout << tmp << endl;
	 tmp = r.pointAtParameter(t2.x());
	cout << tmp << endl;
	 tmp = r.pointAtParameter(t2.y());
	cout << tmp << endl;
	 tmp = r.pointAtParameter(t2.z());
	cout << tmp << endl;
	cout << "end tmp\n";*/
	//cout << tmin << "   " << tmax << endl;
	
	if (tnear > tfar) return false;
	Vec3f mid = r.pointAtParameter((tnear + tfar) / 2);
	if (mid < _min || mid > _max)
		return false;

	if (tfar < tm) {
		swap(tnear, tfar);
		Vec3f tmp = _min;
		_min = _max;
		_max = tmp;
	}
	return true;
}


bool Grid::intersect(const Ray& r, Hit& h, float tmin) {
	//cout << 11 << endl;
	MarchingInfo minfo;
	bool isit = false;
	bool isitplane = false;
	if (!gridvisualize) {

		for (auto pl : planes) {

			isitplane |= pl->intersect(r, h, tmin);
		}
	}

	if (!initializeRayMarch(minfo, r, tmin)) {
		
		if (isitplane) { return true; }
		return false;
	}

	//mark the object
	unordered_map<Object3D*, bool> isHited;
	
	//minfo.print();
	//cout <<"direction"<< r.getDirection() << endl;

	Vec3f dir = r.getDirection();
	//cout << "DIRECTION:  " << dir << endl;
	//minfo.print();
	//cout << 111111 << endl;
	int i, j, k;
	minfo.getCube(i, j, k);
	//cout << "normal:  " <<i<<' '<<j<<' '<<k << endl;
	bool isfst = true;
	float ant=0;
	int anovlp=0;
	Vec3f annormal;
	Object3D* tmpobj;
	Vec3f v1, v2, v3, v4, v5, v6, v7, v8, normal;
	bool opa = false;
	int ovlp,numobj;
	int count = 0;



	while (i < nx && j < ny && k < nz && i >= 0 && j >= 0 && k >= 0) {
		opa = getVoxel(i, j, k);
		numobj = getObjNum(i, j, k);
		//cout << ovlp << endl;
		ovlp = numobj % 10;
		v1 = getPoint(i, j, k);
		v2 = getPoint(i, j + 1, k);
		v3 = getPoint(i + 1, j + 1, k);
		v4 = getPoint(i + 1, j, k);
		v5 = getPoint(i, j, k + 1);
		v6 = getPoint(i, j + 1, k + 1);
		v7 = getPoint(i + 1, j + 1, k + 1);
		v8 = getPoint(i + 1, j, k + 1);

		// paint cube
		RayTree::AddHitCellFace(v1, v2, v6, v5, Vec3f(-1, 0, 0), Mt[ovlp]);
		RayTree::AddHitCellFace(v4, v3, v7, v8, Vec3f(1, 0, 0), Mt[ovlp]);
		RayTree::AddHitCellFace(v1, v4, v8, v5, Vec3f(0, -1, 0), Mt[ovlp]);
		RayTree::AddHitCellFace(v2, v3, v7, v6, Vec3f(0, 1, 0), Mt[ovlp]);
		RayTree::AddHitCellFace(v1, v2, v3, v4, Vec3f(0, 0, -1), Mt[ovlp]);
		RayTree::AddHitCellFace(v5, v6, v7, v8, Vec3f(0, 0, 1), Mt[ovlp]);
		normal = minfo.getNormal();

		// paint entered face
		if (normal == Vec3f(-1, 0, 0))
			RayTree::AddEnteredFace(v1, v2, v6, v5, normal, Mt[ovlp]);
		else if (normal == Vec3f(1, 0, 0))
			RayTree::AddEnteredFace(v4, v3, v7, v8, normal, Mt[ovlp]);
		else if (normal == Vec3f(0, -1, 0))
			RayTree::AddEnteredFace(v1, v4, v8, v5, normal, Mt[ovlp]);
		else if (normal == Vec3f(0, 1, 0))
			RayTree::AddEnteredFace(v2, v3, v7, v6, normal, Mt[ovlp]);
		else if (normal == Vec3f(0, 0, -1))
			RayTree::AddEnteredFace(v1, v2, v3, v4, normal, Mt[ovlp]);
		else if (normal == Vec3f(0, 0, 1))
			RayTree::AddEnteredFace(v5, v6, v7, v8, normal, Mt[ovlp]);
		//如果要显示grid
		if (gridvisualize) {
			//如果不透明且是第一次命中，则更新最终所需参数
			if (opa && isfst) {
				isfst = false;
				ant = minfo.getTmin();
				anovlp = ovlp;
				annormal = normal;
				//如果碰到第一个不透明的就退出，则break
				if (stopquick)
					break;
			}
		//debug
		/*if (opa) {
				for (int t = 0; t < numobj; t++)
				{
					tmpobj = getObj(i, j, k, t);
					isit |= tmpobj->intersect(r, h, tmin);
				}
				//if (isit) return true;
			}*/

		}
		else {
			//如果不显示grid,则此时使用ray_cast_fast
			if (opa) {
				for (int t = 0; t < numobj; t++)
				{
					tmpobj = getObj(i, j, k, t);
					if (isHited.find(tmpobj) == isHited.end()) {
						isit |= tmpobj->intersect(r, h, tmin);
						isHited[tmpobj] = true;
					
					}
				
				}
			}

		}
		minfo.nextCell();
		minfo.getCube(i, j, k);
		//cout << i << j << k << endl;
	};

	/*for (auto ttt : isHited) {
		cout << ttt.first << endl;
	}*/

	if (gridvisualize) {
		if (!opa) return false;
		if (ant < tmin) return false;
		h.set(ant, Mt[anovlp], annormal, r);
		return true;
	}
	else {
		//计算和平面的交点
		
		if (isitplane || isit) { return true; }
		return false;
	}
}