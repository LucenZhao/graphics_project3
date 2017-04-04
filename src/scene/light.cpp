#include <cmath>

#include "light.h"

double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation( const vec3f& P ) const
{
	vec3f dir = getDirection(vec3f(0, 0, 0));
	ray r(P, dir);
	vec3f shadow;
	if (scene->shadown_intersect(r, shadow, DBL_MAX))
	{
		return shadow;
	}
    return vec3f(1,1,1);
}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	double a = m_a;
	double b = m_b;
	double c = m_c;
	double d = (position - P).length();

	if (a == 0 && b == 0 & c == 0)
	{
		return 1.0;
	}
	return min<double>(1, 1.0 / (a + b * d + b * d * d));
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
	vec3f dir = getDirection(P);
	ray r(P, dir);
	vec3f shadow;
	double threshold = (position - P).length();

	if (scene->shadow_intersect(r, shadow, threshold))
	{
		return shadow;
	}

    return vec3f(1,1,1);
}