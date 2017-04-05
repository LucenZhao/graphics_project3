#define _USE_MATH_DEFINES
#include <cmath>

#include "light.h"
#include "../ui/TraceUI.h"

extern TraceUI* traceUI;

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
	if (scene->shadow_intersect(r, shadow, DBL_MAX))
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

vec3f PointLight::shadowAttenuationHelper(const vec3f& P, const ray& r) const
{
	vec3f shadow;
	double threshold = (position - P).length();

	if (scene->shadow_intersect(r, shadow, threshold))
	{
		return shadow;
	}

	return vec3f(1, 1, 1);
}

vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
	// Soft shadow
	if (traceUI->getSoftShadow())
	{
		vec3f shadow(1.0, 1.0, 1.0);
		vec3f softlight_pos;
		for (int i = 0; i < softDepth; i++)
		{
			softlight_pos[0] = position[0] + softExtend * ((double)i / softExtend - 0.5);
			for (int j = 0; j < softDepth; j++)
			{
				softlight_pos[1] = position[1] + softExtend * ((double)j / softExtend - 0.5);
				for (int k = 0; k < softDepth; k++)
				{
					softlight_pos[2] = position[2] + softExtend * ((double)k / softExtend - 0.5);
					ray r(P, (softlight_pos - P).normalize());
					shadow += shadowAttenuationHelper(P, r);
				}
			}
		}
		return shadow / (pow(softDepth, 3));
	}
	else
	{
		return shadowAttenuationHelper(P, ray(P, getDirection(P)));
	}
}

 
double SpotLight::distanceAttenuation(const vec3f& P) const
{
	return 1.0;
}

vec3f SpotLight::getDirection(const vec3f& P) const
{
	return (position - P).normalize();
}

vec3f SpotLight::getColor(const vec3f& P) const
{
	vec3f dir = getDirection(P);

	if (spotAngle != 0)
	{
		if (dir * direction < cos(spotAngle * M_PI / 180)) 
			return vec3f(0, 0, 0);
	}

	// Warn model used in the spotlight
	//	 use cone to control the spotlight
	double center_light = direction * dir;
	center_light = center_light < 0 ? 0 : pow(center_light, spotExponent);
	return center_light * color;
}

vec3f SpotLight::shadowAttenuation(const vec3f& P) const
{
	vec3f dir = getDirection(P);
	vec3f shadow;
	if (scene->shadow_intersect(ray(P, dir), shadow, (position - P).length()))
	{
		return shadow;
	}
	return vec3f(1, 1, 1);
}
