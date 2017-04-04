#ifndef __QUADRIC_H__
#define __QUADRIC_H__

#include "../scene/scene.h"
#include <vector>

class Quadric
	: public MaterialSceneObject
{
public:
	typedef vector<vec3f> Vertices;
	Quadric(Scene *scene, Material *mat, TransformNode *transform, Vertices v)
		: MaterialSceneObject(scene, mat)
	{
		this->transform = transform;
		vertices = v;
	}

	virtual bool intersectLocal(const ray& r, isect& i) const;
	bool intersectFace(const ray& r, isect& i) const;
	virtual bool hasBoundingBoxCapability() const { return true; }
	virtual BoundingBox ComputeLocalBoundingBox()
	{
		BoundingBox localbounds;
		localbounds.max = vec3f(0.5, 0.5, 0.5);
		localbounds.min = vec3f(-0.5, -0.5, -0.5);
		return localbounds;
	}
protected:
	Vertices vertices;
};

#endif // __QUADRIC_H__
