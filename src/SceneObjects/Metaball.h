#ifndef __METABALL_H__
#define __METABALL_H__

#include "../scene/scene.h"

class Metaball
	: public MaterialSceneObject
{
public:
	Metaball(Scene *scene, Material *mat)
		: MaterialSceneObject(scene, mat)
	{
	}

	virtual bool intersectLocal(const ray& r, isect& i) const;
	virtual bool hasBoundingBoxCapability() const { return true; }
	virtual BoundingBox ComputeLocalBoundingBox() const
	{
		BoundingBox localbounds;
		localbounds.max = vec3f(0.5, 0.5, 0.5);
		localbounds.min = vec3f(-0.5, -0.5, -0.5);
		return localbounds;
	}
};
#endif // __SPHERE_H__
