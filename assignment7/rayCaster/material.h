#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "ray.h"
#include "hit.h"
#include "glCanvas.h"  

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

	// CONSTRUCTORS & DESTRUCTOR
	Material(const Vec3f& d_color) { diffuseColor = d_color; }
	Material() { diffuseColor = Vec3f(0,0,0); }
	virtual ~Material() {}

	// ACCESSORS
	virtual Vec3f getDiffuseColor() const { return diffuseColor; }
	virtual Vec3f getReflectiveColor() const { return Vec3f(0,0,0); }
	virtual Vec3f getSpecularColor() const { return Vec3f(0,0,0); }
	virtual Vec3f getTransparentColor() const { return Vec3f(0, 0, 0); }
	virtual float getIndexOfRefraction() { return 1.0; }
	virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,
		const Vec3f& lightColor) const = 0;
	virtual void glSetMaterial() const = 0;
	

protected:

	// REPRESENTATION
	Vec3f diffuseColor;

};

// ====================================================================
// ====================================================================

class PhongMaterial :public Material{
public:
	PhongMaterial(const Vec3f& diffuseColor,
		const Vec3f& specularColor,
		float exponent,
		const Vec3f& reflectiveColor,
		const Vec3f& transparentColor,
		float indexOfRefraction):Material(diffuseColor){
		this->reflectiveColor = reflectiveColor;
		this->transparentColor = transparentColor;
		this->indexOfRefraction = indexOfRefraction;
		this->specularColor = specularColor;
		this->exponent = exponent;
	}
	PhongMaterial(const Vec3f& diffuseColor):Material(diffuseColor)
	{
		specularColor = Vec3f(0.6, 0.6, 0.6);
		exponent = 1;
		transparentColor = Vec3f(0, 0, 0);
		indexOfRefraction = 1;
		reflectiveColor = Vec3f(0, 0, 0);
	}
	Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,
		const Vec3f& lightColor) const;
	void glSetMaterial() const;
	
	Vec3f getReflectiveColor() const { return reflectiveColor; }
	Vec3f getSpecularColor() const { return specularColor; }
	Vec3f getTransparentColor() const { return transparentColor; }
	float getIndexOfRefraction() { return indexOfRefraction; }
private:
	Vec3f specularColor;
	Vec3f reflectiveColor;
	Vec3f transparentColor;
	float indexOfRefraction;
	float exponent;
};


class Checkerboard :public Material {
public:
	Checkerboard(Matrix*M,Material*mat1,Material*mat2):Material(Vec3f(0,0,0)) {
		this->M = M;
		this->mat1 = mat1;
		this->mat2 = mat2;
	}
	Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,
		const Vec3f& lightColor) const;
	void glSetMaterial() const;
private:
	Material* mat1, * mat2;
	Matrix* M;
};

class Noise :public Material {
public:
	Noise(Matrix* m, Material* mat1, Material* mat2, int octaves):Material(Vec3f(0,0,0)) {
		this->M = m;
		this->mat1 = mat1;
		this->mat2 = mat2;
		this->octaves = octaves;
	}
private:
	Material* mat1, * mat2;
	Matrix* M;
	int octaves;
	Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,
		const Vec3f& lightColor) const;
	void glSetMaterial() const;
};
class Marble :public Material {
public:
	Marble(Matrix* m, Material* mat1,
		Material* mat2, int octaves,
		float frequency, float amplitude) {
		this->M = m;
		this->mat1 = mat1;
		this->mat2 = mat2;
		this->frequency = frequency;
		this->octaves = octaves;
		this->amplitude = amplitude;
	}
	Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,
		const Vec3f& lightColor) const;
	void glSetMaterial() const;
private:
	Material* mat1, * mat2;
	Matrix* M;
	int octaves;
	float frequency;
	float amplitude;
};

class Wood :public Material {
public:
	Wood(Matrix* m, Material* mat1,
		Material* mat2, int octaves,
		float frequency, float amplitude) {
		this->M = m;
		this->mat1 = mat1;
		this->mat2 = mat2;
		this->frequency = frequency;
		this->octaves = octaves;
		this->amplitude = amplitude;
	}
	Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,
		const Vec3f& lightColor) const;
	void glSetMaterial() const;
private:
	Material* mat1, * mat2;
	Matrix* M;
	int octaves;
	float frequency;
	float amplitude;
};


#endif