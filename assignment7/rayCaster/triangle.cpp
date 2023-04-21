#include"triangle.h"
#include"grid.h"
#include"raytracing_stats.h"
#define TIGHT_TRIANGLE_BOUND 1
const float epsilon = 1e-5;


Vec3f Triangle::BCcoord(Vec3f point) {
	Vec3f e1, e2;
	e1 = b - a;
	e2 = c - a;
	Vec4f cox;
	Vec3f ans;
	Vec3f tmp = point - a;
	float c1, c2;
	c1 = tmp.Dot3(e1);
	c2 = tmp.Dot3(e2);
	cox.Set(c1, c2, 0, 0);
	M.Transform(cox);
	ans.Set(1 - cox.x() - cox.y(), cox.x(), cox.y());
	return ans;
}
bool Triangle::intersect(const Ray& r, Hit& h, float tmin) {
	RayTracingStats::IncrementNumIntersections();

	Vec3f R0 = r.getOrigin();
	Vec3f Rd = r.getDirection();
	Vec3f tmp = a - R0;
	float f1, f2;
	f1 = tmp.Dot3(normal);
	f2 = Rd.Dot3(normal);
	float t = f1 / f2;
	if (t < tmin) return false;
	Vec3f point = r.pointAtParameter(t);
	Vec3f bccoord = BCcoord(point);
	if (!inTriangle(bccoord)) return false;
	if (t < h.getT())
	{
		h.set(t, m, normal, r);
		//cout << "hit tmin: " << tmin << endl;
		//cout << "hit triangle's normal: " <<
		//	normal << endl;
		//cout << "hit t: " << t << endl << endl;
		return true;
	}
	return false;
}

void Triangle::paint()
{
	m->glSetMaterial();
	glBegin(GL_TRIANGLES);
	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(a.x(), a.y(), a.z());
	glVertex3f(b.x(), b.y(), b.z());
	glVertex3f(c.x(), c.y(), c.z());
	glEnd();
	
}

void Triangle::insertIntoGrid(Grid* g, Matrix* m) {
	if (m != NULL)
	{
		Object3D* tmpobj = new Transform((*m), this);

#if !TIGHT_TRIANGLE_BOUND
		this->Object3D::insertIntoGrid(g, m);
		return;
#else
		Vec3f tmpa, tmpb, tmpc,_min,_max;
		tmpa = a; tmpb = b; tmpc = c;
		
		m->Transform(tmpa);
		m->Transform(tmpb);
		m->Transform(tmpc);

		BoundingBox tmp(tmpa, tmpa);
		tmp.Extend(tmpb);
		tmp.Extend(tmpc);

		_min = tmp.getMin();
		_max = tmp.getMax();

		setBoundingBox(_min, _max);

		Vec3f low, high;
		low = g->getCubeLow(_min);
		high = g->getCubeLow(_max);
		int ix, tx, iy, ty, iz, tz;
		low.GetInt(ix, iy, iz);
		high.GetInt(tx, ty, tz);

		for (int i = ix; i <= tx; i++) {
			for (int j = iy; j <= ty; j++) {
				for (int k = iz; k <= tz; k++) {
					g->setVoxel(i, j, k, true);
					g->setObj(i, j, k, tmpobj);
				}
			}
		}
		return;


#endif
	}

// very tight, when it is not transformed, can be also used when transform is applied.

	

	BoundingBox* box = getBoundingbox();
	
	Vec3f _low, _high;
	Vec3f tmpa = a, tmpb = b, tmpc = c;
	if (m) {
		m->Transform(tmpa);
		m->Transform(tmpb);
		m->Transform(tmpc);
		box->Set(tmpa, tmpa);
		box->Extend(tmpb);
		box->Extend(tmpc);
	}


	Vec3f _min = box->getMin();
	Vec3f _max = box->getMax();


	int lx, ly, lz, hx, hy, hz;
	_low = g->getCubeLow(_min);
	_high = g->getCubeLow(_max);

	lx = _low.x(); ly = _low.y(); lz = _low.z();
	hx = _high.x(); hy = _high.y(); hz = _high.z();
	//cout << _low << " " << _high << endl;
	
	for (int i = lx; i <= hx; i++) {
		for (int j = ly; j <= hy; j++) {
			for (int k = lz; k <= hz; k++) {
				//cube���� i��j, k
				//����������Ƿ񴩹�cube
				//��cube��8���㶼��������֮�ϻ�֮�£��򲻴���
				
				//cube���½�����(cube����ϵ)Ϊi,j,k
				

				//cout <<"ijk: "<< i << ' ' << j << ' ' << k << endl;
				Vec3f p;
				int flgb = 0;
				int flgs = 0;
				for(int ax=0; ax < 2; ax++)
					for(int ay=0; ay < 2; ay++)
						for (int az = 0; az < 2; az++) {
							p = g->getPoint(i+ax, j+ay, k+az);
							p = p - tmpa;
							float tpp = p.Dot3(normal);
							if (tpp > 0)
								flgb += 1;
							else if(tpp<0)
								flgs -= 1;
							
						}
				if (!(flgb==8||flgs==8)) {
					g->setVoxel(i, j, k, true);
					g->setObj(i, j, k, this);
				}
			}
		}
	}
}