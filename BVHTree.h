#ifndef RT_SAMPLE_NEW_BVHTREE_H
#define RT_SAMPLE_NEW_BVHTREE_H

#include "LiteMath.h"
#include "Geometry.h"
using namespace HydraLiteMath;

class BVHTree {
public:

	BVHTree(const float3& a, const float3& x, const float3& y, const float3& z) : a(a), x(x), y(y), z(z), rightVolume(NULL),
	leftVolume(NULL), boundedObjects(NULL), rootElement(NULL), depth(0){};

	~BVHTree() = default;

	bool Intersect(const Ray& ray, float& t_min) const;
	void buildTree(const std::vector<std::shared_ptr<GeoObject>>& geo);
	void getBoundedObjects(Ray& ray, std::vector<int>& boundVector);

	std::vector<int>boundedObjects;

	BVHTree* rightVolume;
	BVHTree* leftVolume;

private:
	float3 a;
	float3 x;
	float3 y;
	float3 z;
	int depth;
	BVHTree* rootElement;

};
#endif //RT_SAMPLE_NEW_BVHTREE_H