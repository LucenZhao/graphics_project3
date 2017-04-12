#include <cmath>
#include <assert.h>

#include "Box.h"

bool Box::intersectLocal( const ray& r, isect& i ) const
{
	// YOUR CODE HERE:
    // Add box intersection code here.
	// it currently ignores all boxes and just returns false.
	double T1 = -DBL_MAX;
	double T2 = DBL_MAX;
	isect all_i[3];
	//bool intersect[3] = { false, false, false };
	//if (intersectXYplane(r, all_i[0], T1, T2)) intersect[0] = true;
	//if (intersectYZplane(r, all_i[1], T1, T2)) intersect[1] = true;
	//if (intersectYZplane(r, all_i[2], T1, T2)) intersect[2] = true;
	//if (!intersect[0] && !intersect[1] && !intersect[2]) return false;
	
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();
	
	for (size_t j = 0; j < 3; j++)
	{
		if (d[j] == 0.0 && (p[j] < -0.5 || p[j] > 0.5)) return false;

		double t1;
		double t2;

		if (d[j] >= 0.0) {
			t1 = (-0.5 - p[j]) / d[j];
			t2 = (0.5 - p[j]) / d[j];
		}
		else {
			t1 = (0.5 - p[j]) / d[j];
			t2 = (-0.5 - p[j]) / d[j];
		}

		if (t2 < T2) T2 = t2;

		if (t1 > T1) {
			T1 = t1;
			i.N = vec3f(0.0, 0.0, 0.0);
			if (d[j] >= 0.0) {
				// Intersection with cap at z = 0.
				i.N[j] = -1.0;
			}
			else {
				i.N[j] = 1.0;
			}
		}
		if (T1 > T2 || T2 < RAY_EPSILON) return false;

	}
	
	if (T1 >= RAY_EPSILON) i.t = T1;
	else i.t = T2;
	i.obj = this;
	return true;
}

bool Box::intersectXYplane(const ray& r, isect& i, double& T1, double& T2) const
{
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();

	if (d[2] == 0.0 && (p[2] < -0.5 || p[2] > 0.5)) return false;

	double t1;
	double t2;

	if (d[2] > 0.0) {
		t1 = (-0.5 - p[2]) / d[2];
		t2 = (0.5 - p[2]) / d[2];
	}
	else {
		t1 = (0.5 - p[2]) / d[2];
		t2 = (-0.5 - p[2]) / d[2];
	}
	
	if (t2 < RAY_EPSILON) {
		return false;
	}
	if (t2 < T2) T2 = t2;
	if (t1 >= RAY_EPSILON) {
		if (t1 > T1) T1 = t1;
		vec3f p2(r.at(t1));
		if (p2[0] > -0.5 && p2[0] < 0.5 && p2[1] > -0.5 && p2[1] < 0.5) {  // inside the face of box
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
				i.N = vec3f(0.0, 0.0, -1.0);
			}
			else {
				i.N = vec3f(0.0, 0.0, 1.0);
			}
			return true;
		}
	

	return false;
}


bool Box::intersectYZplane(const ray& r, isect& i, double& T1, double& T2) const
{
	
	vec3f p = r.getPosition();
	vec3f d = r.getDirection();

	if (d[0] == 0.0) return false;

	double t1;
	double t2;

	if (d[0] > 0.0) {
		t1 = (-0.5 - p[0]) / d[0];
		t2 = (0.5 - p[0]) / d[0];
	}
	else {
		t1 = (0.5 - p[0]) / d[0];
		t2 = (-0.5 - p[0]) / d[0];
	}
	if (t2 < RAY_EPSILON) {
		return false;
	}
	if (t2 < T2) T2 = t2;
	if (t1 >= RAY_EPSILON) {
		if (t1 > T1) T1 = t1;
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
			i.N = vec3f(-1.0, 0.0, 0.0);
		}
		else {
			i.N = vec3f(1.0, 0.0, 0.0);
		}
		return true;
	}

	return false;
	
}

bool Box::intersectXZplane(const ray& r, isect& i, double& T1, double& T2) const
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
		t2 = (-0.5 - p[1]) / d[1];
	}
	if (t2 < RAY_EPSILON) {
		return false;
	}
	if (t2 < T2) T2 = t2;
	if (t1 >= RAY_EPSILON) {
		if (t1 > T1) T1 = t1;
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
			i.N = vec3f(0.0, -1.0, 0.0);
		}
		else {
			i.N = vec3f(0.0, 1.0, 0.0);
		}
		return true;
	}

	return false;
}