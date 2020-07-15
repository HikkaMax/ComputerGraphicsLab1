#ifndef RT_SAMPLE_NEW_GEOMETRY_H
#define RT_SAMPLE_NEW_GEOMETRY_H

#include "LiteMath.h"
#include "RayTracer.h"
#include "Material.h"

using namespace HydraLiteMath;


//Базовый виртуальный класс для геометрического объекта, с которым может пересекаться луч
class GeoObject {
public:
    virtual bool Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const = 0;

	virtual float3 getCenter() const = 0;
	virtual float getMinY() const = 0;
	virtual float getMaxX() const = 0;
	virtual float getMinX() const = 0;
	virtual float getMaxY() const = 0;
	virtual float getMaxZ() const = 0;
	virtual float getMinZ() const = 0;

    std::shared_ptr<Material> m_ptr;

    explicit GeoObject(Material *a_m) : m_ptr(a_m) {};

    virtual ~GeoObject() = default;
};

//Бесконечная плоскость
class Plane : public GeoObject {
public:
    Plane(const float3 a_point, const float3 a_normal, Material *a_m) : GeoObject(a_m), point(a_point),
                                                                        normal(a_normal) {}

    ~Plane() = default;

    bool Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const override;
    float3 getCenter()  const override;
	float getMinY()  const override;
	float getMaxX()  const override;
	float getMinX()  const override;
	float getMaxY()  const override;
	float getMaxZ()  const override;
	float getMinZ()  const override;

private:
    float3 point;
    float3 normal;
};


class Sphere : public GeoObject {
public:

    Sphere(const float3 &a_center, const float &a_r, Material *a_m) : GeoObject(a_m), center(a_center), r(a_r),
                                                                      r_sq(a_r * a_r) {}

    ~Sphere() = default;

    bool Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const override;
	float3 getCenter()  const override;
	float getMinY()  const override;
	float getMaxX()  const override;
	float getMinX()  const override;
	float getMaxY()  const override;
	float getMaxZ()  const override;
	float getMinZ()  const override;

private:
    float3 center;
    float r;
    float r_sq;

};

class Triangle : public GeoObject {
public:
    Triangle(const float3 &dotA, const float3 &dotB, const float3 &dotC, Material *a_m) : GeoObject(a_m), dotA(dotA),
                                                                                          dotB(dotB), dotC(dotC) {}

    ~Triangle() = default;

    bool Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const override;
	float3 getCenter()  const override;
	float getMinY()  const override;
	float getMaxX()  const override;
	float getMinX()  const override;
	float getMaxY()  const override;
	float getMaxZ()  const override;
	float getMinZ()  const override;

private:
    float3 dotA;
    float3 dotB;
    float3 dotC;

};

class Box : public GeoObject {
public:
    Box(const float3 &a, const float3 &x, const float3 &y, const float3 &z, Material *a_m) : GeoObject(a_m), a(a), x(x), y(y), z(z) {}

    ~Box() = default;

    bool Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const override;
	float3 getCenter()  const override;
	float getMinY()  const override;
	float getMaxX()  const override;
	float getMinX()  const override;
	float getMaxY()  const override;
	float getMaxZ()  const override;
	float getMinZ()  const override;

private:
    float3 a;
    float3 x;
    float3 y;
    float3 z;
};

class Cylinder : public GeoObject {
public:
	Cylinder(const float3& center, const float& radius, const float& height, Material* a_m) : GeoObject(a_m), center(center), radius(radius), height(height) {}
	
	~Cylinder() = default;

	bool Intersect(const Ray& ray, float t_min, float t_max, SurfHit &surf) const override;

	bool intersect_base(const Ray& ray, const float3& p, float& t, float3 &normal) const;

	float3 normal_in(const float3& p) const;

private:
	float3 center;
	float3 center2;
	float radius;
	float height;
};


#endif //RT_SAMPLE_NEW_GEOMETRY_H
