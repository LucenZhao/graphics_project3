#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

// The main ray tracer.

#include "scene/scene.h"
#include "scene/ray.h"

class TraceUI;


class RayTracer
{
public:
    RayTracer();
    ~RayTracer();

    vec3f trace( Scene *scene, double x, double y, vec3f bgColor = vec3f(0.0, 0.0, 0.0), vec3f textureColor = vec3f(0.0, 0.0, 0.0));
	vec3f trace(Scene *scene, double x, double y, int depth, vec3f bgColor = vec3f(0.0, 0.0, 0.0), vec3f textureColor = vec3f(0.0, 0.0, 0.0));
	vec3f traceRay( Scene *scene, const ray& r, const vec3f& thresh, int depth, vec3f bg = vec3f(0.0, 0.0, 0.0), vec3f texture = vec3f(0.0, 0.0, 0.0), double intensity = 1.0, bool in = true );

	void setTraceUI(TraceUI* ui) { m_pUI = ui; }
	void getBuffer( unsigned char *&buf, int &w, int &h );
	double aspectRatio();
	void traceSetup( int w, int h );
	void traceLines( int start = 0, int stop = 10000000 );
	void tracePixel( int i, int j );
	void pretracing();

	bool loadScene( char* fn );

	bool sceneLoaded();

private:
	bool		distReflection;
	bool		distRefraction;
	double		distSize;
	int			distRays;
	double		distStep;

	unsigned char *buffer;
	int buffer_width, buffer_height;
	int bufferSize;
	Scene *scene;

	vec3f *pretracing_buffer;
	TraceUI* m_pUI;
	bool m_bSceneLoaded;
};

#endif // __RAYTRACER_H__
