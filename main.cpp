#include <iostream>
#include <vector>
#include <random>

#include "LiteMath.h"
#include "Geometry.h"
#include "Camera.h"
#include "BVHTree.h"

using namespace HydraLiteMath;

	
void RenderScene(uint32_t w, uint32_t h, uint32_t num_samples, const std::vector<std::shared_ptr<GeoObject>> &scene, const Camera &cam, const std::string &filename)
{
	auto dot_light = float3(0.3f, 20.5f, 40.1f);
  auto background_color = float3(0.5f, 0.7f, 1.0f);
  auto film = std::make_unique<Film>(w, h, num_samples);
  auto tracer = std::make_unique<SimpleRT>(16, background_color);

  float invWidth  = 1.0f / float(w);
  float invHeight = 1.0f / float(h);
  BVHTree mainVolume(float3(-15.0f, -8.0f, -12.0f), float3(24.0f, 0.0f, 0.0f), float3(0.0f, 24.0f, 0.0f), float3(0.0f, 0.0f, 24.0f));
  for (int i = 0; i < scene.size(); ++i) {
	  mainVolume.boundedObjects.push_back(i);
  }
  mainVolume.buildTree(scene);

  for (int y = 0; y < h; ++y)
  {
    for (int x = 0; x < w; ++x)
    {
		float3 pixel_color;
      for (int s = 0; s < num_samples; ++s)
      {
        Ray ray = cam.genRay(w, h, x, h - y); //генерируем луч из камеры через текущий пиксель
		int depth = 0;
        pixel_color = tracer->TraceRay(ray, scene, dot_light, depth, mainVolume); //трассируем луч и получаем цвет
      }

      pixel_color /= film->num_samples;      // усредняем полученные цвета
      pixel_color *= cam.getExposureTime();  // умножаем на время экспонирования сенсора - выдержка виртуальной камеры

      film->SetPixelColor(x, y, pixel_color); //записываем цвет на виртуальный сенсор
    }
  }

  film->SaveImagePPM(filename); //сохраняем картинку

}

void test_scene1()
{
  std::vector<std::shared_ptr<GeoObject>> myScene;

 auto plane1 = std::make_shared<Plane>(float3(+10.0f, -1.0f, +0.0f), float3(0.0f, 1.0f, 0.0f), new IdealMirror(float3(0.3f, 0.3f, 0.3f)));
  auto triangle = std::make_shared<Triangle>(float3(+10.0f, +0.0f, +0.0f), float3(+6.0f, +12.0f, +0.0f), float3(+12.0f, +0.0f, +0.0f), new IdealMirror(float3(1.0f, 0.0f, 0.0f)));
  auto sph  = std::make_shared<Sphere> (float3(5.0f, +5.0f, +2.0f), 2,   new DiffusalMaterial(float3(0.0f, 1.0f, 0.0f)));
 auto box  = std::make_shared<Box> (float3(-5.0f, +3.0f, +0.0f),float3(+5.0f, +0.0f, +0.0f), float3(+0.0f, +5.0f, +0.0f), float3(+0.0f, 0.0f, +6.0f),  new DiffusalMaterial(float3(0.0f, 1.0f, 0.0f)));
// auto box  = std::make_shared<Cylinder> (float3(-3.0f, +1.0f, +12.0f), 1.0f, 2.0f,  new IdealMirror(float3(0.0f, 1.0f, 0.0f)));
  auto sph2 = std::make_shared<Sphere> (float3(-288.0f, -288.0f, -376.0f), 1,   new IdealMirror(float3(0.90f, 0.76f, 0.46f)));
  auto sph3 = std::make_shared<Sphere> (float3(+5.0f, +3.0f, -5.0f), 1.5, new IdealMirror(float3(0.65f, 0.77f, 0.97f)));
 auto sph4 = std::make_shared<Sphere> (float3(-3.0f, +1.5f, +5.0f), 1.5, new IdealMirror(float3(0.9f, 0.9f, 0.9f)));
  myScene.push_back(plane1);
  myScene.push_back(sph);
 myScene.push_back(sph2);
  myScene.push_back(sph3);
 myScene.push_back(sph4);
  myScene.push_back(triangle);
  myScene.push_back(box);


  float3 eye   (0.0f, 2.0f, 20.0f);
  float3 lookat(0.0f, 2.0f, 0.0f);
  float3 up    (0.0f, 1.0f, 0.0f);
  float field_of_view = 90.0f;
  unsigned int w = 640;
  unsigned int h =  480;
  Camera cam(eye, lookat, up, field_of_view, float(w) / float(h));

  RenderScene(w, h, 1, myScene, cam,  "test_scene1");
}


void random_scene(int n_objects)
{
  std::vector<std::shared_ptr<GeoObject>> myScene;

  auto plane1 = std::make_shared<Plane>(float3(+0.0f, -1.0f, +0.0f), float3(0.0f, 1.0f, 0.0f), new IdealMirror(float3(0.3f, 0.3f, 0.3f)));
  myScene.push_back(plane1);

  int k = floor(sqrt(n_objects));

  std::random_device rand;
  std::mt19937 rng(rand());
  std::uniform_real_distribution<float> gen(0.0f, 1.0f);

  for(int i = 0; i < n_objects; ++i)
  {
    float3 center(-10.0f + 20.0f * gen(rng), 10.0f * gen(rng), -30.0f  +  40.0f * gen(rng));
    auto sph = std::make_shared<Sphere>(center, 0.05f + 3.0f * gen(rng), new IdealMirror(float3(gen(rng), gen(rng), gen(rng))));
    myScene.push_back(sph);
  }

  float3 eye   (0.0f, 2.0f, 20.0f);
  float3 lookat(0.0f, 2.0f, 0.0f);
  float3 up    (0.0f, 1.0f, 0.0f);
  float field_of_view = 90.0f;
  unsigned int w = 640;
  unsigned int h =  480;
  Camera cam(eye, lookat, up, field_of_view, float(w) / float(h));

//  RenderS scene(w, h, 1, myScene, cam,  "random_scene");
}

int main()
{
  test_scene1();
  random_scene(100);

  return 0;
}
