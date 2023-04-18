#include"raytracer.h"
#include<cmath>
#include<iostream>


using namespace std;
const float epsilon = 1e-4;
const float eps = 0.1;
const float dsw = 1.5;

Vec3f RayTracer::getPhongColor(Ray& r, Hit& h) {
	Light* light;
	Material* m = h.getMaterial();
	int lightn = s->getNumLights();
	float distanceToLight;
	float ft;
	bool isht = false;
	//Vec3f v = r.getDirection();
	//v.Negate();
	Vec3f acolor = s->getAmbientLight();
	Vec3f objcolor = m->getDiffuseColor();
	Vec3f color(0, 0, 0);
	Vec3f lightcolor;
	Vec3f dir;
	Vec3f hitpoint = h.getIntersectionPoint();
	Vec3f tmpcolor;
	Vec3f norm = h.getNormal();
	ft = r.getDirection().Dot3(norm);
	if (shade_back ||ft < 0)
		color += objcolor * acolor;
	
	for (int i = 0; i < lightn; i++) {
		Hit tmp(FLT_MAX, NULL, Vec3f(0, 0, 0));
		
		light = s->getLight(i);
		light->getIllumination(hitpoint, dir, lightcolor, distanceToLight);
		if (shadows) {
			
		//计算与光线之间的第一个与物体的交点存到tmp里
			Ray ptol(hitpoint, dir);
			if (!gridvisualize)
				isht = (s->getGroup())->intersect(ptol, tmp, epsilon);
			else
				isht = (g->intersect(ptol, tmp, epsilon));
			//tmp的t应被初始化为极大
			isht &= (tmp.getT() < distanceToLight);
			if (store_in_tree) {
				raytree->AddShadowSegment(ptol, 0, tmp.getT());
				//cout << i << ' '<<tmp.getT()<<endl;
			}

		}


		tmpcolor.Set(0, 0, 0);

		//如果没有开启shadow模式或者开启了但是没有交点(即有光照)，就计算局部光照
		//否则tmpcolor=0;
		if (!shadows || (shadows && (!isht))) {
			//如果要显示背面并且norm方向相反就反转之后显示
			if (shade_back && ft > 0)
			{
				norm.Negate();
				Hit tp = h;
				tp.setNorm(norm);
				tmpcolor = m->Shade(r, tp, dir, lightcolor);
			}
			else
				tmpcolor = m->Shade(r, h, dir, lightcolor);
			//cout << tmpcolor << endl;
		}
		else {
			//开启了shadows且有交点(无光线直射)
#if OPTION_TRANSPARENT_SHADOWS == 1
			if (shade_back && ft > 0)
			{
				norm.Negate();
				Hit tp = h;
				tp.setNorm(norm);
				tmpcolor = m->Shade(r, tp, dir, lightcolor);
			}
			else
				tmpcolor = m->Shade(r, h, dir, lightcolor);


			Vec3f tmphitpoint = tmp.getIntersectionPoint();
			Ray tmpr(tmphitpoint, dir);
			tmp.set(FLT_MAX, NULL, Vec3f(0, 0, 0));
			isht = (s->getGroup())->intersect(tmpr, tmp, epsilon);
			isht &= (tmp.getT() < distanceToLight);
			while (isht) {
				/*if (store_in_tree) {
					raytree->AddShadowSegment(tmpr, 0, tmp.getT());
					//cout << i << ' '<<tmp.getT()<<endl;
				}*/
				//cout << tmphitpoint <<"    ";
				//当前的入射光在物体内部
				//cout << tmp.getT() << endl;
				if (tmp.getNormal().Dot3(dir) > 0) {
					Material* M = tmp.getMaterial();
					float dis = tmp.getT();
					Vec3f pw = M->getTransparentColor() * (1 / (dis*dsw + epsilon));
					pw.Clamp(0.,1.);
					tmpcolor = tmpcolor * pw;
					//光强随透射距离衰减
					
				}
				tmphitpoint = tmp.getIntersectionPoint();
				tmpr.reSet(tmphitpoint, dir);
				tmp.set(FLT_MAX, NULL, Vec3f(0, 0, 0));
				isht = (s->getGroup())->intersect(tmpr, tmp, epsilon);
				isht &= (tmp.getT() < distanceToLight);
				//cout << tmp.getT() << endl;
				//cout << dir << endl;
			}
			//cout << "------------\n";
#endif // OPTION_TRANSPARENT_SHADOWS == 1

		}


		//cout << endl;
		/*if (tmpcolor.Length() == 0)
			cout << h.getIntersectionPoint() << endl;*/
		color += tmpcolor;
	}
	
	return color;
}


Vec3f RayTracer::traceRay(Ray& ray, float tmin, int bounces, float weight,
	float indexOfRefraction, Hit& hit) {
	//cout << weight << endl;
	if (!gridvisualize) {
		if (!(s->getGroup())->intersect(ray, hit, tmin))
		{
			//cout << "no hit"<<s->getBackgroundColor();
			return s->getBackgroundColor();
		}
	}
	else {
		if (!(g->intersect(ray, hit, tmin)))
		{
			//cout << "no hit"<<s->getBackgroundColor();
			return s->getBackgroundColor();
		}
	}
	Vec3f color;
	Vec3f refRaydir, trsRaydir;
	Vec3f intPoint = hit.getIntersectionPoint();
	Vec3f incoming = ray.getDirection();
	Vec3f norm = hit.getNormal();
	Material* m = hit.getMaterial();
	Vec3f reflectiveColor = m->getReflectiveColor();
	Vec3f transparentColor = m->getTransparentColor();
	//cout << transparentColor << endl;
	color = getPhongColor(ray, hit);
	//cout << color << endl;
	Hit ref_hit(FLT_MAX, NULL, Vec3f(0, 0, 0));
	Hit trs_hit(FLT_MAX, NULL, Vec3f(0, 0, 0));
	float index_i = 1.0;
	float index_t = 1.0;
	Vec3f tmpcolor;
	if (reflectiveColor.Length() > 0 &&
		(bounces < max_bounces && weight >= cutoff_weight)) {
		refRaydir = mirrorDirection(norm, incoming);
		Ray refRay(intPoint, refRaydir);
		tmpcolor = reflectiveColor * traceRay(refRay, epsilon, bounces + 1, weight * reflectiveColor.Length()/sqrtf(3.0), indexOfRefraction, ref_hit);
		color += tmpcolor;
		//cout << tmpcolor <<' '<<color<< endl;
		if (store_in_tree) raytree->AddReflectedSegment(refRay, 0, ref_hit.getT());
	}
	if (transparentColor.Length() > 0 &&
		(bounces < max_bounces && weight >= cutoff_weight)) {
		float flg = norm.Dot3(incoming);
		if (flg < 0)
			index_t = m->getIndexOfRefraction();
		else {
			index_i = m->getIndexOfRefraction();
			norm.Negate();
		}
		bool ists = transmittedDirection(norm, incoming, index_i, index_t, trsRaydir);
		if (ists)
		{
			Ray trsRay(intPoint, trsRaydir);
			color += transparentColor * traceRay(trsRay, epsilon, bounces + 1, weight * transparentColor.Length()/sqrtf(3.0), index_t, trs_hit);
			if (store_in_tree) raytree->AddTransmittedSegment(trsRay, 0, trs_hit.getT());
		}
	}


	//cout << bounces<<' '<<color << endl;
	return color;

}



Vec3f RayTracer::mirrorDirection(const Vec3f& normal, const Vec3f& incoming) {
	Vec3f dy = normal.Dot3(incoming) * normal;
	dy.Negate();
	Vec3f ans = incoming + dy * 2;
	ans.Normalize();
	return ans;
}

bool RayTracer::transmittedDirection(const Vec3f& normal, const Vec3f& incoming,
	float index_i, float index_t, Vec3f& transmitted) {
	float nr = index_i / index_t;
	Vec3f ans;
	float ctheta = -normal.Dot3(incoming);
	float t2 = 1 - nr * nr * (1 - ctheta * ctheta);
	if (t2 < 0) return false;
	ans = (nr * ctheta - sqrt(t2)) * normal + nr * incoming;
	ans.Normalize();
	transmitted = ans;
	return true;
}