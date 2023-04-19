#include<cmath>
#include <GL/glut.h>
#include <GL/gl.h>
#include<algorithm>
#include"material.h"
#include"vectors.h"
#include "ray.h"
#include "hit.h"
#include "glCanvas.h" 
#include "matrix.h"
#include"perlin_noise.h"
#include<cmath>
#define GETaix(v) (v).x(),(v).y(),(v).z()

Vec3f PhongMaterial::Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,
	const Vec3f& lightColor) const {
	Vec3f v = ray.getDirection();
	v.Negate();
	Vec3f h = dirToLight + v;
	h.Normalize();
	Vec3f n = hit.getNormal();
	float x = n.Dot3(h);
	x = powf(x, exponent);
	Vec3f ans;
	float fl = dirToLight.Dot3(n);
	if (fl > 0)
#if !SPECULAR_FIX 
		ans = specularColor * x * lightColor;
#else
		ans = specularColor * x * lightColor * fl;
#endif
	else
		ans.Set(0, 0, 0);

	x = n.Dot3(dirToLight);
	x = max(float(0), x);
	ans += x * diffuseColor * lightColor;
	return ans;
}

void PhongMaterial::glSetMaterial(void) const {

	GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat specular[4] = {
	  getSpecularColor().r(),
	  getSpecularColor().g(),
	  getSpecularColor().b(),
	  1.0 };
	GLfloat diffuse[4] = {
	  getDiffuseColor().r(),
	  getDiffuseColor().g(),
	  getDiffuseColor().b(),
	  1.0 };

	// NOTE: GL uses the Blinn Torrance version of Phong...      
	float glexponent = exponent;
	if (glexponent < 0) glexponent = 0;
	if (glexponent > 128) glexponent = 128;

#if !SPECULAR_FIX 

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

#else

	// OPTIONAL: 3 pass rendering to fix the specular highlight 
	// artifact for small specular exponents (wide specular lobe)

	if (SPECULAR_FIX_WHICH_PASS == 0) {
		// First pass, draw only the specular highlights
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);

	}
	else if (SPECULAR_FIX_WHICH_PASS == 1) {
		// Second pass, compute normal dot light 
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
	}
	else {
		// Third pass, add ambient & diffuse terms
		assert(SPECULAR_FIX_WHICH_PASS == 2);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
	}

#endif
}


Vec3f Checkerboard::Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,
	const Vec3f& lightColor) const {
	Vec3f hitpoint = hit.getIntersectionPoint();
	M->Transform(hitpoint);
	float af, bf, cf;
	hitpoint.Get(af, bf, cf);
	int a, b, c;
	a = (int)floor(af);
	b = (int)floor(bf);
	c = (int)floor(cf);
	if ((a + b + c) & 1)
		return mat2->Shade(ray, hit, dirToLight, lightColor);
	else
		return mat1->Shade(ray, hit, dirToLight, lightColor);

}
void Checkerboard::glSetMaterial() const{
	mat1->glSetMaterial();
}
Vec3f Noise::Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,
	const Vec3f& lightColor) const{
	Vec3f hitpoint = hit.getIntersectionPoint();
	M->Transform(hitpoint);
	float pr = 0;
	float pt = 0;
	float powr = 1;
	for (int i = 0; i < octaves; i++) {
		pr += (float)PerlinNoise::noise(GETaix(hitpoint))/(powr);
		powr *= 2;
		hitpoint *= 2;
	}
	//cout << pr << endl;
	if (pr < 0) pr = -pr;

	return (mat1->Shade(ray, hit, dirToLight, lightColor) * (pr)
		+ mat2->Shade(ray, hit, dirToLight, lightColor) * (1-(pr)));
}

void Noise::glSetMaterial() const {
	mat1->glSetMaterial();
}


Vec3f Marble::Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,
	const Vec3f& lightColor) const {
	Vec3f hitpoint = hit.getIntersectionPoint();
	M->Transform(hitpoint);
	float x = hitpoint.x();
	float pr = 0;
	int powr = 1;
	for (int i = 0; i < octaves; i++) {
		pr += (float)PerlinNoise::noise(GETaix(hitpoint)) / (powr);
		powr *= 2;
		hitpoint *= 2;
	}
	pr = sin(frequency * x + amplitude * pr);
	return mat1->Shade(ray, hit, dirToLight, lightColor) * pr
		+ mat2->Shade(ray, hit, dirToLight, lightColor) * (1 - pr);
}
void Marble::glSetMaterial() const {
	mat1->glSetMaterial();
}


Vec3f Wood::Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,
	const Vec3f& lightColor) const {
	Vec3f hitpoint = hit.getIntersectionPoint();
	M->Transform(hitpoint);
	float d = hitpoint.z()* hitpoint.z() + hitpoint.x()* hitpoint.x();
	d = sqrt(d);
	float pr = 0;
	int powr = 1;
	for (int i = 0; i < octaves; i++) {
		pr += (float)PerlinNoise::noise(GETaix(hitpoint)) / (powr);
		powr *= 2;
		hitpoint *= 2;
	}
	pr = sin( d*frequency  + amplitude * pr);
	return mat1->Shade(ray, hit, dirToLight, lightColor) * pr
		+ mat2->Shade(ray, hit, dirToLight, lightColor) * (1 - pr);
}
void Wood::glSetMaterial() const {
	mat1->glSetMaterial();
}