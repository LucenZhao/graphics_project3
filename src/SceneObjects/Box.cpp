#include <cmath>
#include <assert.h>

#include "Box.h"

bool Box::intersectLocal( const ray& r, isect& i ) const
{
	// YOUR CODE HERE:
    // Add box intersection code here.
	// it currently ignores all boxes and just returns false.
	i.obj = this;
	isect ixy, iyz, ixz;
	if (intersectXYplane(r, ixy) || intersectYZplane(r, iyz) || intersectXZplane(r, ixz))
	{
		i = ixy;
		i.obj = this;
		if (iyz.t < i.t) {
			i = iyz;
			i.obj = this;
		}
		if (ixz.t < i.t) {
			i = ixz;
			i.obj = this;
		}
		return true;
	}
	return false;
}

bool Box::intersectXYplane(const ray& r, isect& i) const
{
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();

	if (d[2] == 0.0) return false;

	double t1;
	double t2;

	if (d[2] > 0.0) {
		t1 = (-0.5 -p[2]) / d[2];
		t2 = (0.5 - p[2]) / d[2];
	}
	else {
		t1 = (0.5 - p[2]) / d[2];
		t2 = (-0.5 -p[2]) / d[2];
	}
	if (t2 < RAY_EPSILON) {
		return false;
	}
	if (t1 >= RAY_EPSILON) {
		vec3f p2(r.at(t1));
		if (p2[0] > -0.5 && p2[0] < 0.5 && p2[1] > -0.5 && p2[1] < 0.5) {
			i.t = t1;
			if (d[2] > 0.0) {
				// Intersection with cap at z = 0.
				i.N = vec3f(0.0, 0.0, -1.0);
			}
			else {
				i.N = vec3f(0.0, 0.0, 1.0);
			}
			return true;
		}
	}

	vec3f p2(r.at(t2));
	if (p2[0] > -0.5 && p2[0] < 0.5 && p2[1] > -0.5 && p2[1] < 0.5) {
		i.t = t2;
		if (d[2] > 0.0) {
			// Intersection with cap at z = 1.
			i.N = vec3f(0.0, 0.0, 1.0);
		}
		else {
			i.N = vec3f(0.0, 0.0, -1.0);
		}
		return true;
	}

	return false;
}


bool Box::intersectYZplane(const ray& r, isect& i) const
{
	
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();

	if (d[0] == 0.0) return false;

	double t1;
	double t2;

	if (d[0] > 0.0) {
		t1 = (-0.5 -p[0]) / d[0];
		t2 = (0.5 - p[0]) / d[0];
	}
	else {
		t1 = (0.5 - p[0]) / d[0];
		t2 = (-0.5 -p[0]) / d[0];
	}
	if (t2 < RAY_EPSILON) {
		return false;
	}
	if (t1 >= RAY_EPSILON) {
		vec3f p2(r.at(t1));
		if (p2[2] > -0.5 && p2[2] < 0.5 && p2[1] > -0.5 && p2[1] < 0.5) {
			i.t = t1;
			if (d[0] > 0.0) {
				// Intersection with cap at z = 0.
				i.N = vec3f(-1.0, 0.0, 0.0);
			}
			else {
				i.N = vec3f(1.0, 0.0, 0.0);
			}
			return true;
		}
	}

	vec3f p2(r.at(t2));
	if (p2[2] > -0.5 && p2[2] < 0.5 && p2[1] > -0.5 && p2[1] < 0.5) {
		i.t = t2;
		if (d[0] > 0.0) {
			// Intersection with cap at z = 1.
			i.N = vec3f(1.0, 0.0, 0.0);
		}
		else {
			i.N = vec3f(-1.0, 0.0, 0.0);
		}
		return true;
	}

	return false;
	
}

bool Box::intersectXZplane(const ray& r, isect& i) const
{
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();

	if (d[1] == 0.0) return false;

	double t1;
	double t2;

	if (d[1] > 0.0) {
		t1 = (-0.5 -p[1]) / d[1];
		t2 = (0.5 - p[1]) / d[1];
	}
	else {
		t1 = (0.5 - p[1]) / d[1];
		t2 = (-0.5 -p[1]) / d[1];
	}
	if (t2 < RAY_EPSILON) {
		return false;
	}
	if (t1 >= RAY_EPSILON) {
		vec3f p2(r.at(t1));
		if (p2[2] > -0.5 && p2[2] < 0.5 && p2[0] > -0.5 && p2[0] < 0.5) {
			i.t = t1;
			if (d[1] > 0.0) {
				// Intersection with cap at z = 0.
				i.N = vec3f(0.0, -1.0, 0.0);
			}
			else {
				i.N = vec3f(0.0, 1.0, 0.0);
			}
			return true;
		}
	}

	vec3f p2(r.at(t2));
	if (p2[2] > -0.5 && p2[2] < 0.5 && p2[0] > -0.5 && p2[0] < 0.5) {
		i.t = t2;
		if (d[1] > 0.0) {
			// Intersection with cap at z = 1.
			i.N = vec3f(0.0, 1.0, 0.0);
		}
		else {
			i.N = vec3f(0.0, -1.0, 0.0);
		}
		return true;
	}

	return false;
}