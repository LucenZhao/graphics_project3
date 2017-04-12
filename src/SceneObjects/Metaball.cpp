#include <cmath>

#include "Metaball.h"

bool Metaball::intersectLocal(const ray& r, isect& i) const
{
	vec3f v = -r.getPosition();
	double b = v.dot(r.getDirection());
	double discriminant = b*b - v.dot(v) + 1;

	if (discriminant < 0.0) {
		return false;
	}

	discriminant = sqrt(discriminant);
	double t2 = b + discriminant;

	if (t2 <= RAY_EPSILON) {
		return false;
	}

	i.obj = this;

	double t1 = b - discriminant;

	if (t1 > RAY_EPSILON) {
		i.t = t1;
		i.N = r.at(t1).normalize();
		if (i.N * r.getDirection() > 0)
			i.N = -i.N;
	}
	else {
		i.t = t2;
		i.N = r.at(t2).normalize();
		if (i.N * r.getDirection() > 0)
			i.N = -i.N;
	}

	return true;
}
