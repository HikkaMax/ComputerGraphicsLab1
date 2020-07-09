#include "Geometry.h"


bool Plane::Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const {
    surf.t = dot((point - ray.o), normal) / dot(ray.d, normal);

    if (surf.t > t_min && surf.t < t_max) {
        surf.hit = true;
        surf.hitPoint = ray.o + surf.t * ray.d;
        surf.normal = normal;
        surf.m_ptr = m_ptr;
        return true;
    }   

    return false;
}

/////////////////////////////////////////


//Для пересечения со сферой подставляем луч в уравнение сферы и решаем получившееся квадратное уравнение
//Решения этого уравнения - точки пересечения луча со сферой
bool Sphere::Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const {

    float3 k = ray.o - center;

    float a = dot(ray.d, ray.d);
    float b = dot(2 * k, ray.d);
    float c = dot(k, k) - r_sq;

    float d = b * b - 4 * a * c;

    if (d < 0) return false;

    surf.t = (-b - sqrt(d)) / 2 * a;

    if (surf.t > t_min && surf.t < t_max) {
        surf.hit = true;
        surf.hitPoint = ray.o + surf.t * ray.d;
        surf.normal = normalize(surf.hitPoint - center);
        surf.m_ptr = m_ptr;
        return true;
    }

    surf.t = (-b + sqrt(d)) / 2 * a;
    if (surf.t > t_min && surf.t < t_max) {
        surf.hit = true;
        surf.hitPoint = ray.o + surf.t * ray.d;
        surf.normal = normalize(surf.hitPoint - center);
        surf.m_ptr = m_ptr;
        return true;
    }

    return false;
}

bool Triangle::Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const {
    float3 normal = cross(dotB - dotA, dotC - dotA);
    float3 T = ray.o - dotA;
    float3 E1 = dotB - dotA;
    float3 E2 = dotC - dotA;

    surf.t = dot(cross(T, E1), E2) / (dot(cross(ray.d, E2), E1));
    float u = dot(cross(ray.d, E2), T) / (dot(cross(ray.d, E2), E1));
    float v = dot(cross(T, E1), ray.d) / (dot(cross(ray.d, E2), E1));

    if (((u >= 0) && (v >= 0) && ((u + v) <= 1)) && (surf.t > t_min) && (surf.t < t_max)) {
        surf.hit = true;
        surf.hitPoint = ray.o + surf.t * ray.d;
        surf.normal = normal;
        surf.m_ptr = m_ptr;
        return true;

    }

    return false;
}

bool Box::Intersect(const Ray &ray, float t_min, float t_max, SurfHit &surf) const {
    float3 b = a + x + y + z;

    float3 tA = (a - ray.o) / ray.d;
    float3 tB = (b - ray.o) / ray.d;

    float tMin = max(max(min(tA.x, tB.x), min(tA.y, tB.y)), min(tA.z, tB.z));
    float tMax = min(min(max(tA.x, tB.x), max(tA.y, tB.y)), max(tA.z, tB.z));
    if (tMin <= tMax && tMax > 0) {
        surf.t = tMin;
        surf.hit = true;
        surf.hitPoint = ray.o + surf.t * ray.d;
        if (abs(surf.hitPoint.x - a.x) < 0.001f)
            surf.normal = float3(+1.0f, +0.0f, +0.0f);
        if (abs(surf.hitPoint.x - b.x) < 0.001f)
            surf.normal = float3(+1.0f, +0.0f, +0.0f);

        if (abs(surf.hitPoint.y - a.y) < 0.001f)
            surf.normal = float3(+0.0f, +1.0f, +0.0f);
        if (abs(surf.hitPoint.y - b.y) < 0.001f)
            surf.normal = float3(+0.0f, +1.0f, +0.0f);

        if (abs(surf.hitPoint.z - a.z) < 0.001f)

            surf.normal = float3(+0.0f, +0.0f, +1.0f);
        if (abs(surf.hitPoint.z - b.z) < 0.001f)
            surf.normal = float3(+0.0f, +0.0f, +1.0f);
        surf.m_ptr = m_ptr;
        return true;
    }

    return false;
}

bool Cylinder::Intersect(const Ray& ray, float t_min, float t_max, SurfHit &surf) const
{// translate the ray origin

	float3 p0(ray.o.x - center.x, ray.o.y - center.y, ray.o.z - center.z);

	// coefficients for the intersection equation
	// got them mathematically intersecting the line equation with the cylinder equation
	float a = ray.d.x * ray.d.x + ray.d.z * ray.d.z;
	float b = ray.d.x * p0.x + ray.d.z * p0.z;
	float c = p0.x * p0.x + p0.z * p0.z - radius * radius;

	float delta = b * b - a * c;

	// delta < 0 means no intersections
	if (delta < t_min)
		return false;

	// nearest intersection
	surf.t = (-b - sqrt(delta)) / a;

	// t<0 means the intersection is behind the ray origin
	// which we don't want
	if (surf.t <= t_min)
		return false;


	float y = p0.y + ray.d.y;

	// check if we intersect one of the bases
	if (y < center.y + height + t_min || y > center.y - t_min) {
		float dist;
		float3 normal;
		bool b1 = intersect_base(ray, float3(center.x, center.y + height, center.z), dist, normal);
		if (b1) {
			surf.t = dist;
			surf.normal = normal;
			surf.hitPoint = ray.o + surf.t * ray.d;
		}
		bool b2 = intersect_base(ray, center, dist, normal);
		if (b2 && dist > t_min&& surf.t >= dist) {
			surf.t = dist;
			surf.normal = normal;
			surf.hitPoint = ray.o + surf.t * ray.d;
		}

		if (b1 || b2) {
			surf.hit = true;
			surf.m_ptr = m_ptr;
			return true;
		};

	}
	return false;

}

// Calculate intersection with the base having center c
// We do this by calculating the intersection with the plane
// and then checking if the intersection is within the base circle
bool Cylinder::intersect_base(const Ray& ray, const float3& c, float& t, float3 &normal) const
{
	normal = normal_in(c);
	float3 p0(ray.o.x - center.x, ray.o.y - center.y, ray.o.z - center.z);
	float A = normal.x;
	float B = normal.y;
	float C = normal.z;
	float D = -(A * (c.x - center.x) + B * (c.y - center.y) + C * (c.z - center.z));

	if (A * ray.d.x + B * ray.d.y + C * ray.d.z == 0)
		return false;

	float dist = -(A * p0.x + B * p0.y + C * p0.z + D) / (A * ray.d.x + B * ray.d.y + C * ray.d.z);

	float t_min = 0.01;

	if (dist < t_min)
		return false;

	float3 p;
	p.x = p0.x + dist * ray.d.x;
	p.y = p0.y + dist * ray.d.y;
	p.z = p0.z + dist * ray.d.z;
	if (p.x * p.x + p.z * p.z - radius * radius > t_min)
		return false;

	t = dist;

	return true;
}

float3 Cylinder::normal_in(const float3& p) const
{
	// Point is on one of the bases
	if (p.x<center.x + radius && p.x>center.x - radius && p.z<center.z + radius && p.z>center.z - radius)
	{
		float t_min = 0.01;
		if (p.y < center.y + height + t_min && p.y>center.y + height - t_min) {
			return float3(0, 1, 0);
		}
		if (p.y < center.y + t_min && p.y>center.y - t_min) {
			return float3(0, -1, 0);
		}
	}

	// Point is on lateral surface
	float3 c0(center.x, p.y, center.z);
	float3 v = p - c0;
	return normalize(v);
}
