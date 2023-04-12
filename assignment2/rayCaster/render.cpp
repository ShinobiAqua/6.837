#include"render.h"
void Render::basic_rendering(Image& img) {
	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f point;
	int scalen;
	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height - 1;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width - 1;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			point.Set(i, j);
			point += start;
			point /= scalen;
			//cout << point.x()<<' '<<point.y() << endl;
			Ray r = camera->generateRay(point);
			Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
			bool ismx = group->intersect(r, h, camera->getTMin());

			if (ismx) {
				Material* mtr = h.getMaterial();
				img.SetPixel(i, j, mtr->getDiffuseColor());
			}
			else {
				img.SetPixel(i, j, backgroundcolor);
			}
		}
}

void Render::depth_rendering(Image& img,float depth_min,float depth_max) {
	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f point;
	int scalen;
	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height - 1;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width - 1;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			point.Set(i, j);
			point += start;
			point /= scalen;
			//cout << point.x()<<' '<<point.y() << endl;
			Ray r = camera->generateRay(point);
			Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
			bool ismx = group->intersect(r, h, camera->getTMin());
			if (ismx) {
				float t = h.getT();
				float gry = (depth_max - t) / float(depth_max - depth_min);
				img.SetPixel(i, j, Vec3f(gry, gry, gry));
			}
			else {
				img.SetPixel(i, j, Vec3f(0,0,0));
			}
		}
}

void Render::normal_rendering(Image& img) {
	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f point;
	int scalen;
	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height - 1;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width - 1;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			point.Set(i, j);
			point += start;
			point /= scalen;
			//cout << point.x()<<' '<<point.y() << endl;
			Ray r = camera->generateRay(point);
			Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
			bool ismx = group->intersect(r, h, camera->getTMin());

			if (ismx) {
				Vec3f color = h.getNormal();
				color.Abs();
				img.SetPixel(i, j, color);
			}
			else {
				img.SetPixel(i, j, backgroundcolor);
			}
		}
}

void Render::diffuse_rendering(Image& img) {
	int width, height;
	width = img.Width();
	height = img.Height();
	Vec2f start;
	Vec2f point;
	Vec3f color;
	int scalen;
	if (height > width)
	{
		start.Set(float(height - width) / 2, 0); scalen = height - 1;
	}
	else
	{
		start.Set(0, float(width - height) / 2); scalen = width - 1;
	}

	for (int i = 0; i < width; i++)
		for (int j = 0; j < height; j++) {
			point.Set(i, j);
			point += start;
			point /= scalen;
			//cout << 1 << endl;
			//cout << point.x()<<' '<<point.y() << endl;
			Ray r = camera->generateRay(point);
			/*Vec3f dir = r.getDirection();
			cout << dir << endl;*/
			Hit h(FLT_MAX, NULL, Vec3f(0, 0, 0));
			bool ismx = group->intersect(r, h, camera->getTMin());

			if (ismx) {
				color = getColor(h);
				img.SetPixel(i, j,color);
			}
			else {
				//cout << 1 << endl;
				img.SetPixel(i, j, backgroundcolor);
			}
		}
}