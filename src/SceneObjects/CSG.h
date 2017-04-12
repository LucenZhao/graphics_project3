#ifndef __CSG_H__
#define __CSG_H__

#include "../scene/scene.h"
#include <vector>

enum CSG_RELATION {
	CSG_OR = 1,
	CSG_AND,
	CSG_MINUS
};

class IestPoint {
public:
	double t;
	vec3f normal;
	bool isRed;
	bool operator<(const IestPoint& other) {
		return t < other.t - RAY_EPSILON;
	}
};

class IntersectPoints {
public:
	IntersectPoints() : mPoints() {}
	IntersectPoints& Merge(const IntersectPoints& pIntersectPoints, int relation);
	bool firstPositive(IestPoint& p);
	void addPoint(IestPoint& pt)
	{
		mPoints.push_back(pt);
	}
private:
	vector<IestPoint> mPoints;
};

class CSGNode
{
public:
	CSGNode() : left(NULL), right(NULL), object(NULL), relation(CSG_AND), isLeaf(0) {}
	void setObject(Geometry* obj) { this->object = obj; }
	void setIsLeaf(bool p) { this->isLeaf = p; }
	BoundingBox getBoundingBox() const;
	void computeBoundingBox();
	IntersectPoints* intersectLocal(const ray& r) const;
	CSGNode* left;
	CSGNode* right;
	Geometry* object;
	CSG_RELATION relation;
	bool isLeaf;
	BoundingBox bound;
};

class CSGTree {
public:
	CSGTree() :root(NULL) {}
	CSGTree(const CSGTree& other) {
		root = other.root;
	}
	CSGTree(CSGNode* nodes) : root(nodes) {}
	CSGTree* merge(const CSGTree* pTree, CSG_RELATION relation);
	bool intersect(const ray& r, isect& i) const;
	CSGNode* getRoot() { return root; }
private:
	CSGNode* root;
};

// data structure for CSG geometry, containing a tree structure
// The style is like Cylinder.h
class CSG
	: public MaterialSceneObject
{
public:
	CSG(Scene *scene, Material *mat, CSGTree* tr)
		: MaterialSceneObject(scene, mat), tree(tr)
	{
	}

	virtual bool intersectLocal(const ray& r, isect& i) const;
	virtual bool hasBoundingBoxCapability() const { return true; }
	virtual bool hasInterior() const { return true; }
	virtual BoundingBox ComputeLocalBoundingBox();

private:
	CSGTree* tree;
};
#endif