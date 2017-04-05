// The main ray tracer.

#include <Fl/fl_ask.h>

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"
#include "fileio/read.h"
#include "fileio/parse.h"
#include "ui/TraceUI.h"

#include <random>
#include <iostream>
using namespace std;
// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
	// upgrade the second whistle
	// Implement antialiasing by supersampling and averaging down.
	int super_sampling = m_pUI->getSuperSampling();
	int jitter = m_pUI->getJitter();

	if (super_sampling != 0)
	{
		// super sampling and jittering
		double lower_bound = 0.0;
		double upper_bound = 1.0 * jitter;
		std::uniform_real_distribution<double> random(lower_bound, upper_bound);
		static std::default_random_engine random_engine;

		vec3f result;
		for (int i = 0; i < super_sampling; i++)
		{
			for (int j = 0; j < super_sampling; j++)
			{
				ray r(vec3f(0, 0, 0), vec3f(0, 0, 0));
				double offsetX = ((i + 0.5 + (random(random_engine) - upper_bound / 2)) / super_sampling - 0.5) / buffer_width;
				double offsetY = ((j + 0.5 + (random(random_engine) - upper_bound / 2)) / super_sampling - 0.5) / buffer_height;
				scene->getCamera()->rayThrough(x + offsetX, y + offsetY, r);
				// sum up the trace ray result
				result += traceRay(scene, r, vec3f(1.0, 1.0, 1.0), 0).clamp();
			}
		}
		// avg the super samping result
		return (result / (super_sampling * super_sampling));
	}
	else
	{
		ray r(vec3f(0, 0, 0), vec3f(0, 0, 0));
		scene->getCamera()->rayThrough(x, y, r);
		return traceRay(scene, r, vec3f(1.0, 1.0, 1.0), 0).clamp();
	}
}

void RayTracer::pretracing()
{
	if (pretracing_buffer != NULL)
	{
		delete [] pretracing_buffer;
	}

	pretracing_buffer = new vec3f[((buffer_height + 1) * (buffer_width + 1))];
	for (int y = 0; y < buffer_height + 1; ++y)
	{
		for (int x = 0; x < buffer_width + 1; ++x)
		{
			ray r(vec3f(0, 0, 0), vec3f(0, 0, 0));
			scene->getCamera()->rayThrough((x - 0.5) / buffer_width, (y - 0.5) / buffer_height, r);
			pretracing_buffer[x + y * (buffer_width + 1)] = traceRay(scene, r, vec3f(1.0, 1.0, 1.0), 0).clamp();
		}
	}
}

// Implement antialiasing by adaptive supersampling: II
vec3f RayTracer::trace(Scene *scene, double x, double y, int depth)
{
	double x1 = (x - 0.5) / buffer_width;
	double x2 = (x - 0.5 + 1.0 / pow(2, depth)) / buffer_width;
	double y1 = (y - 0.5) / buffer_height;
	double y2 = (y - 0.5 + 1.0 / pow(2, depth)) / buffer_height;
	
	ray r1(vec3f(0, 0, 0), vec3f(0, 0, 0));
	scene->getCamera()->rayThrough(x1, y1, r1);
	vec3f t1 = traceRay(scene, r1, vec3f(1.0, 1.0, 1.0), 0).clamp();
	ray r2(vec3f(0, 0, 0), vec3f(0, 0, 0));
	scene->getCamera()->rayThrough(x2, y1, r2);
	vec3f t2 = traceRay(scene, r2, vec3f(1.0, 1.0, 1.0), 0).clamp();
	ray r3(vec3f(0, 0, 0), vec3f(0, 0, 0));
	scene->getCamera()->rayThrough(x1, y2, r3);
	vec3f t3 = traceRay(scene, r3, vec3f(1.0, 1.0, 1.0), 0).clamp();
	ray r4(vec3f(0, 0, 0), vec3f(0, 0, 0));
	scene->getCamera()->rayThrough(x2, y2, r4);
	vec3f t4 = traceRay(scene, r4, vec3f(1.0, 1.0, 1.0), 0).clamp();

	// ceratiorion
	double t = 1.0e-4;
	vec3f average = (t1 + t2 + t3 + t4) / 4;
	// recursive on some edge points 
	if (depth < m_pUI->getAdaptiveDepth())
	{
		if ((t1 - average).length() > t ||
			(t2 - average).length() > t ||
			(t3 - average).length() > t ||
			(t4 - average).length() > t)
		{
			vec3f sum = trace(scene, x, y, depth + 1) + trace(scene, x + 1 / pow(2, depth + 1), y, depth + 1) +
				trace(scene, x, y + 1 / pow(2, depth + 1), depth + 1) + trace(scene, x + 1 / pow(2, depth + 1), y + 1 / pow(2, depth + 1), depth + 1);
			return sum / 4;
		}
	}
	return average;
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay( Scene *scene, const ray& r, 
	const vec3f& thresh, int depth, double intensity, bool in )
{
	isect i;

	if( scene->intersect( r, i ) ) {
		const Material& m = i.getMaterial();
		vec3f I = m.shade(scene, r, i);

		vec3f ones(1.0, 1.0, 1.0);
		vec3f ktInv = ones - m.kt;
		I = prod(ktInv, I);

		if (depth < m_pUI->getDepth() && (m_pUI->m_thresholdSlider->value() == 0 || intensity > m_pUI->m_thresholdSlider->value()))
		{
			vec3f r2_position = r.getPosition() + r.getDirection() * i.t;

			// reflection
			if (m.kr[0] > 0 && m.kr[1] > 0 && m.kr[2] > 0)
			{
				// cout << "reflection";
				vec3f reflected_direction = (2 * ((-r.getDirection()) * i.N) * i.N - (-r.getDirection())).normalize();
				if (!distReflection) // distribution reflection off
				{
					ray r2(r2_position, reflected_direction);
					vec3f next_I = traceRay(scene, r2, thresh, depth + 1, (m.ks[0] + m.ks[1] + m.ks[2]) / 3 * intensity, in);

					I += prod(m.ks, next_I);
				}
				else // distribution reflection on
				{

				}
			}
			// transmissive
			if (m.kt[0] > 0 && m.kt[1] > 0 && m.kt[2] > 0)
			{
				//	cout << "refraction" << endl;
				if (i.getMaterial().index != 1.0)
				{
					double eta = in ? 1.0 / i.getMaterial().index : i.getMaterial().index;
					double NI = -r.getDirection() * i.N;
					double cosThetaTsq = 1 - eta * eta * (1 - NI * NI);

					if (cosThetaTsq >= 0) // else total internal refraction
					{
						vec3f refract_direction = (eta * NI - sqrt(cosThetaTsq)) * i.N - eta * -r.getDirection();
						refract_direction = refract_direction.normalize();

						if (!distRefraction) // distribution refraction off
						{
							ray r2(r2_position, refract_direction);
							vec3f next_I = traceRay(scene, r2, thresh, depth + 1, (m.kt[0] + m.kt[1] + m.kt[2]) / 3 * intensity, !in);
							I += prod(m.kt, next_I);
						}
						else // distribution refraction on
						{

						}
					}
				}
				else {
					ray r2(r.getPosition() + r.getDirection() * i.t, r.getDirection());
					vec3f next_I = traceRay(scene, r2, thresh, depth + 1, (m.kt[0] + m.kt[1] + m.kt[2]) / 3 * intensity, !in);
					I += prod(m.kt, next_I);
				}
			}
		}
		return I;
	} 
	else 
	{
		// not intersection
		return vec3f( 0.0, 0.0, 0.0 );
	}
}

RayTracer::RayTracer()
{
	// init
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;
	pretracing_buffer = NULL;

	distReflection = false;
	distRefraction = false;
	distSize = 0;
	distRays = 0;
	distStep = 0;

	m_bSceneLoaded = false;
}


RayTracer::~RayTracer()
{
	delete [] buffer;
	delete scene;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded()
{
	return m_bSceneLoaded;
}

bool RayTracer::loadScene( char* fn )
{
	try
	{
		scene = readScene( fn );
	}
	catch( ParseError pe )
	{
		fl_alert( "ParseError: %s\n", pe );
		return false;
	}

	if( !scene )
		return false;
	
	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[ bufferSize ];
	
	// separate objects into bounded and unbounded
	scene->initScene();
	
	// Add any specialized scene loading code here
	
	m_bSceneLoaded = true;

	return true;
}

void RayTracer::traceSetup( int w, int h )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];
	}
	memset( buffer, 0, w*h*3 );
}

void RayTracer::traceLines( int start, int stop )
{
	vec3f col;
	if( !scene )
		return;

	if( stop > buffer_height )
		stop = buffer_height;

	distReflection = m_pUI->m_distReflSlider->value() > 0;
	distRefraction = m_pUI->m_distRefrSlider->value() > 0;
	distSize = m_pUI->m_distSizeSlider->value();
	distRays = m_pUI->m_distRaysSlider->value();
	distStep = distSize / (distRays - 1);

	for( int j = start; j < stop; ++j )
		for( int i = 0; i < buffer_width; ++i )
			tracePixel(i,j);
}

void RayTracer::tracePixel( int i, int j )
{
	vec3f col;
	if (!scene)
	{
		return;
	}

	// Implement antialiasing by adaptive supersampling: I
	int adaptive_depth = m_pUI->getAdaptiveDepth();
	if (adaptive_depth > 0)
	{
		if (i == 0 && j == 0)
			pretracing();
		vec3f *pb = pretracing_buffer;
		vec3f average = (pb[i + j*(buffer_width + 1)] +
			pb[i + 1 + j*(buffer_width + 1)] +
			pb[i + (j + 1)*(buffer_width + 1)] +
			pb[i + 1 + (j + 1)*(buffer_width + 1)]) / 4;

		// ceratiorion
		double t = 1.0e-4;
		// begin adaptive supersampling
		if ( (average - pb[i + j*(buffer_width + 1)]).length() > t ||
			(average - pb[i + 1 + j*(buffer_width + 1)]).length() > t ||
			(average - pb[i + (j + 1)*(buffer_width + 1)]).length() > t ||
			(average - pb[(i + 1) + (j + 1)*(buffer_width + 1)]).length() > t
			)
		{
			col = trace(scene, i, j, 1) + trace(scene, i + 0.5, j, 1) + trace(scene, i, j + 0.5, 1) + trace(scene, i + 0.5, j + 0.5, 1);
			col = col / 4;
		}
		else
		{
			col = average;
		}
	}
	else
	{
		double x = double(i) / double(buffer_width);
		double y = double(j) / double(buffer_height);

		col = trace(scene, x, y);
	}

	// update each pixel
	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}