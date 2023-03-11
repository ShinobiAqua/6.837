#include"ifs.h"
void IFS::read(char* input_file) {
	FILE* input = fopen(input_file, "r");
	assert(input != NULL);

	// read the number of transforms
	fscanf(input, "%d", &n);

	matx = new Matrix[n];
	prob = new float[n];

	// read in the transforms
	for (int i = 0; i < n; i++) {
		fscanf(input, "%f", &prob[i]);
		matx[i].Read3x3(input);
		// < DO SOMETHING WITH probability and m >
	}
	// close the file
	fclose(input);
}
void IFS::render(Image& img, int lots, int num_iters) {
	int xlen = img.Width();
	int ylen = img.Height();
	Vec3f GREEN(0, 255, 0);
	srand(time(NULL));
	for (int i = 0; i < lots; i++)
	{
		//cout << i << endl;
		//
		float x = rand() % 100 / (float)100;
		float y = rand() % 100 / (float)100;
		Vec2f v(x, y);
		//cout << v.x() << ' ' << v.y() << endl;
		//pic a random point
		for (int k = 0; k < num_iters; k++)
		{
			//cout << k << endl;
			//pic a random transform
			float pr = rand() % 100 / (float)101;
			int ct = 0;
			for (ct=0; ct < n; ct++)
			{
				pr -= prob[ct];
				if (pr < 0) break;
			}
			if (ct == n)
				ct -= 1;
			assert(ct >= 0 && ct<n);
			matx[ct].Transform(v);
		}
		if (v.x() < 0 || v.x() > 1 || v.y() < 0 || v.y() > 1)
		{
			i--;
			continue;
		}
		
		img.SetPixel((int)(v.x() * xlen), (int)(v.y() * ylen), GREEN);
	}
}