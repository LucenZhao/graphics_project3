// The main ray tracer.

#include <Fl/fl_ask.h>

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"
#include "fileio/read.h"
#include "fileio/parse.h"
#include "ui/TraceUI.h"

// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
    ray r( vec3f(0,0,0), vec3f(0,0,0) );
    scene->getCamera()->rayThrough( x,y,r );
	return traceRay( scene, r, vec3f(1.0,1.0,1.0), 0 ).clamp();
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay( Scene *scene, const ray& r, 
	const vec3f& thresh, int depth )
{
	isect i;

	if( scene->intersect( r, i ) ) {
		const Material& m = i.getMaterial();
		vec3f I = m.shade(scene, r, i);

		if (depth < m_pUI->getDepth())
		{
			vec3f r2_position = r.getPosition() + r.getDirection() * i.t;

			// reflection
			if (m.kr[0] > 0 && m.kr[1] > 0 && m.kr[2] > 0)
			{
				vec3f reflected_direction = r.getDirection() - i.N * (2.0 * (r.getDirection() * i.N));

				ray r2(r2_position, reflected_direction);
				vec3f next_I = traceRay(scene, r2, thresh, depth + 1);

				I += prod(m.ks, next_I);
			}

			// transmissive
			if (m.kt[0] > 0 && m.kt[1] > 0 && m.kt[2] > 0)
			{
				vec3f N_ref = i.N;
				double cosi = r.getDirection() * N_ref;
				double etai = 1, etat = m.index;
				if (cosi < 0)
				{
					// outside the surface
					cosi = -cosi;
				}
				else
				{
					// inside the surface
					N_ref = -N_ref;
					// swap the refraction index
					std::swap(etai, etat);
				}

				double eta = etai / etat;
				double sintSquare = 1 - eta * eta * (1 - cosi * cosi);

				if (sintSquare > 0) // else total internal refraction
				{
					vec3f refract_direction = eta * I + (eta * cosi - sqrtf(sintSquare)) * N_ref;
					refract_direction = refract_direction.normalize();

					ray r2(r2_position, refract_direction);
					vec3f next_I = traceRay(scene, r2, thresh, depth + 1);
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
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;

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

	for( int j = start; j < stop; ++j )
		for( int i = 0; i < buffer_width; ++i )
			tracePixel(i,j);
}

void RayTracer::tracePixel( int i, int j )
{
	vec3f col;

	if( !scene )
		return;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	col = trace( scene,x,y );

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}