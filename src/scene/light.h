#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "scene.h"

class Light
	: public SceneElement
{
public:
	virtual vec3f shadowAttenuation(const vec3f& P) const = 0;
	virtual double distanceAttenuation( const vec3f& P ) const = 0;
	virtual vec3f getColor( const vec3f& P ) const = 0;
	virtual vec3f getDirection( const vec3f& P ) const = 0;
	virtual vec3f getAmbientColor(const vec3f& P) const = 0;

protected:
	Light( Scene *scene, const vec3f& col )
		: SceneElement( scene ), color( col ) {}

	vec3f 		color;
};

class AmbientLight
	: public Light
{
public:
	AmbientLight(Scene *scene, const vec3f& color)
		: Light(scene, color) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation(const vec3f& P) const;
	virtual vec3f getDirection(const vec3f& P) const;
	virtual vec3f getColor(const vec3f& P) const;
	virtual vec3f getAmbientColor(const vec3f& P) const;
};

class DirectionalLight
	: public Light
{
public:
	DirectionalLight( Scene *scene, const vec3f& orien, const vec3f& color )
		: Light( scene, color ), orientation( orien ) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;
	virtual vec3f getAmbientColor(const vec3f& P) const;
protected:
	vec3f 		orientation;
};

class PointLight
	: public Light
{
public:
	PointLight(Scene *scene, const vec3f& pos, const vec3f& color, double aa, double bb, double cc)
		: Light(scene, color), position(pos), m_a(aa), m_b(bb), m_c(cc) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	vec3f shadowAttenuationHelper(const vec3f& P, const ray& r, double threshold) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;
	virtual vec3f getAmbientColor(const vec3f& P) const;

	double m_a;
	double m_b;
	double m_c;

protected:
	vec3f position;
	double softExtend = 2.0;
	int softDepth = 6;
};

class SpotLight
	:public Light
{
public:
	SpotLight(Scene *scene, const vec3f& color, const vec3f& pos, double spotExp, vec3f dir, double angle)
		: Light(scene, color), position(pos), spotExponent(spotExp), direction(dir), spotAngle(angle) {}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation(const vec3f& P) const;
	virtual vec3f getColor(const vec3f& P) const;
	virtual vec3f getDirection(const vec3f& P) const;
	virtual vec3f getAmbientColor(const vec3f& P) const;

protected:
	double	spotAngle;
	vec3f	position;
	double	spotExponent;
	vec3f	direction;
};

#endif // __LIGHT_H__
