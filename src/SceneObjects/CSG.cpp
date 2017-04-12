#include <cmath>

#include "CSG.h"

typedef vector<IestPoint>::iterator iter;

IntersectPoints& IntersectPoints::Merge(const IntersectPoints& pIntersectPoints, int relation) {
	vector<IestPoint> tmp;

	for (iter it = mPoints.begin(); it != mPoints.end(); ++it)
	{
		tmp.push_back(*it);
	}
	mPoints.clear();

	vector<IestPoint> pPoints(pIntersectPoints.mPoints);
	for (iter it = pPoints.begin(); it != pPoints.end(); ++it)
	{
		if (relation == CSG_MINUS)
			it->isRed = !it->isRed;
		tmp.push_back(*it);
	}

	int before = 0, after = 0;
	int order = (relation == CSG_AND) ? 2 : 1;
	sort(tmp.begin(), tmp.end());
	
	for (iter it = tmp.begin(); it != tmp.end(); ++it)
	{
		if (it->isRed)
			after += 1;
		else
			after -= 1;

		if (before < order && after >= order) {
			it->isRed = true;
			mPoints.push_back(*it);
		}
		else if (before >= order&&after < order) {
			it->isRed = false;
			mPoints.push_back(*it);
		}
		before = after;
	}
	return *this;
}

bool IntersectPoints::firstPositive(IestPoint& p)
{
	bool flag = false;
	for (iter it = mPoints.begin(); it != mPoints.end(); ++it)
	{
		if (it->t > RAY_EPSILON)
		{
			if (flag)
			{
				if (it->t < p.t) p = *it;
			}
			else
			{
				flag = true;
				p = (*it);
			}
		}
	}
	return flag;
}

CSGTree* CSGTree::merge(const CSGTree* pTree, CSG_RELATION relation) {
	CSGNode* temp = new CSGNode;
	temp->left = root;
	temp->right = pTree->root;
	temp->isLeaf = false;
	temp->relation = relation;
	temp->computeBoundingBox();
	root = temp;
	return this;
}

bool CSGTree::intersect(const ray& r, isect& i) const
{
	if (!root) return false;
	IntersectPoints* inters = root->intersectLocal(r);
	IestPoint sp;
	if (!inters->firstPositive(sp)) return false;
	i.t = sp.t;
	bool t = sp.normal * r.getDirection() > RAY_EPSILON;
	if (!sp.isRed)
	{
		i.N = t ? sp.normal : -sp.normal;
	}
	else
	{
		i.N = t ? -sp.normal : sp.normal;
	}
	return true;
}

IntersectPoints* CSGNode::intersectLocal(const ray& r) const
{
	IntersectPoints* result = new IntersectPoints();
	if (isLeaf)
	{
		isect i;
		IestPoint i1, i2;
		ray r(r.at(-10000), r.getDirection());
		if (!object->intersect(r, i)) 
			return result;
		i1.t = i.t - 10000;
		i1.normal = i.N;
		i1.isRed = true;
		ray r2(r.at(i1.t + RAY_EPSILON * 10), r.getDirection());
		if (!object->intersect(r2, i))
		{
			i2 = i1;
		}
		else
		{
			i2.t = i.t + i1.t;
			i2.normal = i.N;
		}
		i2.isRed = false;
		result->addPoint(i1);
		result->addPoint(i2);
		return result;
	}
	else
	{
		if (!left || !right) 
			return result;
		IntersectPoints* leftSeg = new IntersectPoints();
		IntersectPoints* rightSeg = new IntersectPoints();
		leftSeg = left->intersectLocal(r);
		rightSeg = right->intersectLocal(r);
		leftSeg->Merge(*rightSeg, relation);
		return leftSeg;
	}
}

BoundingBox CSGNode::getBoundingBox() const
{
	return bound;
}

void CSGNode::computeBoundingBox()
{
	if (isLeaf)
	{
		bound = object->getBoundingBox();
	}
	else
	{
		if (!left || !right)
		{
			bound = BoundingBox();
		}
		bound = left->getBoundingBox();
		bound.merge(right->getBoundingBox());
	}
}

bool CSG::intersectLocal(const ray& r, isect& i) const
{
	if (!tree->intersect(r, i)) return false;
	i.obj = this;
	return true;
}

BoundingBox CSG::ComputeLocalBoundingBox() {
	CSGNode* rt = tree->getRoot();
	if (!rt) return BoundingBox();
	return rt->getBoundingBox();
}