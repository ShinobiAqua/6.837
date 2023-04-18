#include"object3d.h"
#include"grid.h"
#include"Matrix.h"
#include"boundingbox.h"
void Object3D::insertIntoGrid(Grid *g,Matrix *m) {
	if (m == NULL) return;
	if (box == NULL) return;
	Vec3f _min, _max;
	_min = box->getMin();
	_max = box->getMax();


	Vec3f v[8];
	float a[6];
	_min.Get(a[0], a[2], a[4]);
	_max.Get(a[1], a[3], a[5]);
	//底部顺时针 v0 v2 v6 v4
	//顶部顺时针 v1 v3 v7 v5
	int ct = 0;
	for (int i = 0; i < 2; i++)
		for (int j = 2; j < 4; j++)
			for (int k = 4; k < 6; k++) {
				v[ct++].Set(a[i], a[j], a[k]);
			}

	for (int i = 0; i < 8; i++)
		m->Transform(v[i]);

	BoundingBox tmp(v[0],v[0]);
	for (int i = 1; i < 8; i++)
		tmp.Extend(v[i]);

	_min = tmp.getMin();
	_max = tmp.getMax();

	/*cout << "before transform\n";
	cout << _min << ' ' << _max << endl;*/

	
	
	/*cout << "after transform\n";
	cout << _min << ' ' << _max << endl << endl;*/

	//m->print();
	
	Vec3f low, high;
	low = g->getCubeLow(_min);
	high = g->getCubeLow(_max);
	int ix, tx, iy, ty, iz, tz;
	low.GetInt(ix, iy, iz);
	high.GetInt(tx, ty, tz);
	/*cout << "low and high :" << ix << ' ' << iy << ' ' << iz << endl;
	cout << tx << ' ' << ty << ' ' << tz << endl;
	cout << "low and high vector: " << low << ' ' << high << endl << endl;*/


	for (int i = ix; i <= tx; i++) {
		for (int j = iy; j <= ty; j++) {
			for (int k = iz; k <= tz; k++) {
				g->setVoxel(i, j, k, true);
				g->setObj(i, j, k, this);
			}
		}
	}

	return;
}