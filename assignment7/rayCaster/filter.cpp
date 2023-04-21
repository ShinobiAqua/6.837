#include "filter.h"
#include"vectors.h"
#include"film.h"
Vec3f Filter::getColor(int i, int j, Film* film) {
	int numSample = film->getNumSamples();
	Vec3f ans(0, 0, 0);
	float offset_x, offset_y;
	Vec3f tmpcolor;
	float w;
	float allw = 0;
	Sample smp;
	int r = getSupportRadius();
	//cout << "before: " << film->getSample(i, j, 0).getColor() << " "<<endl;
	for(int ii = i - r; ii <= i + r; ii++)
		for (int jj = j - r; jj <= j + r; jj++) {
			if (ii<0 || ii>=film->getWidth() || jj<0 || jj>=film->getHeight()) continue;
			for (int t = 0; t < numSample; t++)
			{
				smp = film->getSample(ii, jj, t);
				offset_x = smp.getPosition().x();
				offset_y = smp.getPosition().y();
				//cout << ii << ' ' << jj << ' '<<t<<' ' << offset_x << ' ' << offset_y << endl;
				//offx=ii+offset_x-(i+0.5)
				offset_x = ii + offset_x - (i + 0.5);
				offset_y = jj + offset_y - (j + 0.5);
				tmpcolor = smp.getColor();
				w = getWeight(offset_x,offset_y);
				//cout << w << endl;
				tmpcolor *= w;
				allw += w;
				//cout << w << endl;
				ans += tmpcolor;
			}
			//cout << ans << endl;
		}
	ans /= allw;
	//cout << (r * 2 + 1) * (r * 2 + 1) * numSample << ' ' << allw << endl;
	//cout << "after :"<< ans << ' '<<allw<< endl;
	return ans;
}