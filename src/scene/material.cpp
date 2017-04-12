#include "ray.h"
#include "material.h"
#include "light.h"


// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade( Scene *scene, const ray& r, const isect& i, bool using_texture, vec3f texture ) const
{
	vec3f zero;
	// sum of ambient 
	vec3f sum1;
	for (auto j = scene->beginLights(); j != scene->endLights(); ++j)
	{
		sum1 += prod(ka, (*j)->getAmbientColor(zero));
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
		// vec3f shadowA = vec3f(1, 1, 1);
		//std::cout << "current shadowA is " << shadowA << std::endl;
		double distA = currlight->distanceAttenuation(isectpos);
		if (using_texture) sum2 += prod(shadowA, prod(currlight->getColor(isectpos), (NL * texture + VR * ks) * distA));
		else sum2 += prod(shadowA, prod(currlight->getColor(isectpos), (NL * kd + VR * ks) * distA));
	}

	return ke + sum1 + sum2;
}
 