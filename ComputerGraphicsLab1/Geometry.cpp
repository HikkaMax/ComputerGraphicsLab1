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