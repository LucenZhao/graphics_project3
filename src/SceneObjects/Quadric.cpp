#include <cmath>
#include <assert.h>

#include "Quadric.h"


bool Quadric::intersectLocal(const ray& r, isect& i) const
{
	if (intersectFace(r, i) == true) return true;
	return false;
}

bool Quadric::intersectFace(const ray& r, isect& i) const
{
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();
	double A = d[0] * d[0] + -1 * d[1] * d[1] + d[2] * d[2];
	double B = 2 * (d[0] * p[0] + -1 * d[1] * p[1] + d[2] * p[2]);
	double C = p[0] * p[0] + -1 * p[1] * p[1] + p[2] * p[2] -1;
	double discriminant = B * B - 4 * A * C;

	if (discriminant < 0.0) {
		return false;
	}

	discriminant = sqrt(discriminant);
	double t2 = (-1 * B + discriminant) / (2 * A);

	if (t2 <= RAY_EPSILON) {
		return false;
	}

	i.obj = this;

	double t1 = (-1 * B - discriminant) / (2 * A);

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