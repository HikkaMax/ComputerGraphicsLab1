#include <limits>
#include "RayTracer.h"
#include "Material.h"
#include "Geometry.h"


//Базовый алгоритм трассировки луча
float3 SimpleRT::TraceRay(Ray &ray, const std::vector <std::shared_ptr<GeoObject>> &geo, float3 dot_light, int &depth) {
	auto pixel_color = float3(0.0f, 0.0f, 0.0f);
	while (depth < max_ray_depth) {
		float tnear = std::numeric_limits<float>::max();
		int geoIndex = -1;

		SurfHit surf;
		//среди геометрии сцены ищем объекты, с которыми пересекается текущий луч и находим ближайшее пересечение
		for (int i = 0; i < geo.size(); ++i) {
			SurfHit temp;

			if (geo.at(i)->Intersect(ray, 0.001, tnear, temp)) {
				if (temp.t < tnear) {
					tnear = temp.t;
					geoIndex = i;
					surf = temp;
				}
			}
		}

		//если луч не пересек ни один объект, то значит он улетел в фон
		//вычисляем цвет как градиент цвета фона
		if (geoIndex == -1) {
			pixel_color += bg_color;
			break;
		}

		float3 surfColor(0.0f, 0.0f, 0.0f);

		Ray shadow_ray(surf.hitPoint, normalize(dot_light - surf.hitPoint));

		int shadowGeoIndex = -1;
		for (int i = 0; i < geo.size(); ++i) {
			SurfHit temp;
			if (geo.at(i)->Intersect(shadow_ray, 0.01, tnear, temp)) {
				shadowGeoIndex = i;
			}
		}

		if (shadowGeoIndex == -1) {
			Ray ray_out;
			if (surf.m_ptr->Scatter(ray, surf, surfColor, ray_out, shadow_ray)) {
				ray = ray_out;
			}
			pixel_color += surfColor;
		}
		depth++;
	}
	return pixel_color;
}

//bool isLightSource (std::shared_ptr<GeoObject> element){
//    return (element->m_ptr == DotLight )
//}


//float3 SimpleRT::WhittedRayTrace(const Ray &ray, const std::vector<std::shared_ptr<GeoObject>> &geo, const int &depth){
//    float tnear = std::numeric_limits<float>::max();
//    int geoIndex = -1;
//
//    SurfHit surf;
//    GeoObject lightSource = std::find(geo.begin(),geo.end(),)
//            ))
//    for (int i = 0; i < geo.size(); i++){
//        SurfHit temp;
//
//        if (geo.at(i)->Intersect(ray,0.01, tnear,temp)){
//            surf = temp;
//            Ray shadowRay(ray.d, )
//        }
//    }
//}


