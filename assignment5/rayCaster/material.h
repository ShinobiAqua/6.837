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
		specularColor = Vec3f(0.5, 0.5, 0.5);
		exponent = 1;
		transparentColor = Vec3f(0, 0, 0);
		indexOfRefraction = 1;
		reflectiveColor = Vec3f(0, 0, 0);
	}
	virtual Vec3f Shade(const Ray& ray, const Hit& hit, const Vec3f& dirToLight,
		const Vec3f& lightColor) const;
	virtual void glSetMaterial() const;
	
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

#endif