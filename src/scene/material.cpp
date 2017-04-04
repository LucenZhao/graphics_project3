#include "ray.h"
#include "material.h"
#include "light.h"

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE
	vec3f zero;
	// sum of ambient
	vec3f sum1;
	vec3f ambientColor(0, 0, 0);
	for (auto j = scene->beginLights(); j != scene->endLights(); ++j)
	{
		sum1 += prod(ka, ambientColor);
	}

	// sum of lights
	vec3f sum2;
	vec3f isectpos = r.getPosition() + i.t*r.getDirection();
	for (auto j = scene->beginLights(); j != scene->endLights(); ++j)
	{
		auto currlight = *j;
		if (currlight->getDirection(isectpos).iszero())
			continue;
		vec3f L = currlight->getDirection(isectpos);
		vec3f V = -r.getDirection();

		double NL = (i.N * L);
		vec3f Rm = (2 * NL * i.N - L).normalize();
		double VR = Rm * V;
		if (NL < 0)NL = 0;
		if (VR < 0)VR = 0;
		VR = pow(VR, shininess * 128);

		vec3f H = (L + V).normalize();

		vec3f shadowA = currlight->shadowAttenuation(isectpos);
		double distA = currlight->distanceAttenuation(isectpos);
		sum2 += prod(shadowA, prod(currlight->getColor(isectpos), (NL * kd + VR * ks) * distA));
	}


	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
    // You will need to call both distanceAttenuation() and shadowAttenuation()
    // somewhere in your code in order to compute shadows and light falloff.

	return kd+sum1+sum2;
}
