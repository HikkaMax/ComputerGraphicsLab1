#include "BVHTree.h"

bool BVHTree::Intersect(const Ray& ray, float t_min, float t_max) const
{
	float3 b = a + x + y + z;

	float3 tA = (a - ray.o) / ray.d;
	float3 tB = (b - ray.o) / ray.d;

	float tMin = max(max(min(tA.x, tB.x), min(tA.y, tB.y)), min(tA.z, tB.z));
	float tMax = min(min(max(tA.x, tB.x), max(tA.y, tB.y)), max(tA.z, tB.z));
	if (tMin <= tMax && tMax > 0) {
		return true;
	}

	return false;
}

void BVHTree::buildTree(const std::vector<std::shared_ptr<GeoObject>>& geo)
{
	int inscribedObjectsCount = 0;
	for (auto i : boundedObjects) {
		bool xIntersect = geo.at(i)->getMaxX() == a.x + x.x || geo.at(i)->getMinX() == a.x;
		bool yIntersect = geo.at(i)->getMaxY() == a.y + y.y || geo.at(i)->getMinY() == a.y;
		bool zIntersect = geo.at(i)->getMaxZ() == a.z + z.z || geo.at(i)->getMinZ() == a.z;
		if (xIntersect || yIntersect || zIntersect) {
			inscribedObjectsCount++;
		}
	}
	if (inscribedObjectsCount == boundedObjects.size() || boundedObjects.size() == 0 || depth > 5) {
		return;
	}
	float xBorder = (a.x + x.x) / 2;
	std::vector<int> leftObjects, rightObjects;
	for (auto i : boundedObjects) {
		if (geo.at(i)->getCenter().x > xBorder) {
			rightObjects.push_back(i);
		}
		else {
			leftObjects.push_back(i);
		}
	}
	float lmaxX = a.x;
	float lmaxY = a.y;
	float lmaxZ = a.z;
	for (auto i : leftObjects) {
		if (lmaxX < geo.at(i)->getMaxX()) {
			lmaxX = geo.at(i)->getMaxX();
		}
		if (lmaxY <= geo.at(i)->getMinY()) {
			lmaxY = geo.at(i)->getMinY();
		}
		if (lmaxZ <= geo.at(i)->getMinZ()) {
			lmaxZ = geo.at(i)->getMinZ();
		}
	}
	float rminX = a.x + x.x;
	float rminY = a.y + y.y;
	float rminZ = a.z + z.z;
	for (auto i : rightObjects) {
		if (rminX > geo.at(i)->getMinX()) {
			rminX = geo.at(i)->getMinX();
		}
		if (rminY > geo.at(i)->getMinY()) {
			rminY = geo.at(i)->getMinY();
		}
		if (rminZ > geo.at(i)->getMinZ()) {
			rminZ = geo.at(i)->getMinZ();
		}
	}

	float3 b = a + x + y + z;

	BVHTree* tempLeftVolume = new BVHTree(a,float3(lmaxX, x.y, x.z), float3(y.x, lmaxY, y.z), float3(z.x, z.y, lmaxZ));
	tempLeftVolume->boundedObjects = leftObjects;
	tempLeftVolume->depth = depth + 1;
	tempLeftVolume->buildTree(geo);
	leftVolume = tempLeftVolume;
	float3 rA = float3(rminX, rminY, rminZ);
	BVHTree* tempRightVolume  = new BVHTree(rA, float3(b.x, x.y, x.z), float3(y.x, b.y, y.z), float3(z.x, z.y, b.z));
	tempRightVolume->boundedObjects = rightObjects;
	tempRightVolume->depth = depth + 1;
	tempRightVolume->buildTree(geo);
	rightVolume = tempRightVolume;

	return;
}

 void BVHTree::getBoundedObjects(Ray& ray, std::vector<int>&boundVector)
{
	float tnear = std::numeric_limits<float>::max();
	bool kek =  rightVolume == NULL;
	if (rightVolume == NULL && leftVolume == NULL ) {
		boundVector = boundedObjects;
		return;
	}
	else {
		if (leftVolume != NULL && leftVolume->boundedObjects.size() != 0 && leftVolume->Intersect(ray, 0.001, tnear)) {
			leftVolume->getBoundedObjects(ray, boundVector);
			return;
		}
		else if (rightVolume != NULL && rightVolume->boundedObjects.size() != 0 && rightVolume->Intersect(ray, 0.001, tnear)) {
			rightVolume->getBoundedObjects(ray, boundVector);
			return;
		}
		else {
			return;
		}
	}
}
