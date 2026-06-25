#include "bvh_node.h"
#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "model.h"
#include "ray.h"
#include "rtweekend.h"
#include "sphere.h"
#include "texture.h"
#include "translate.h"
#include "vec3.h"
#include <atomic>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

color ray_color(const ray &r, const vec3 &background, const hittable &world,
                int depth) {
  hit_record rec;

  if (depth <= 0)
    return vec3(0, 0, 0);

  if (!world.hit(r, 0.001, infinity, rec))
    return background;

  vec3 emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
  ray scattered;
  vec3 attenuation;

  if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
    return emitted +
           attenuation * ray_color(scattered, background, world, depth - 1);

  return emitted;
}

void render_chunk(int start_y, int end_y, int image_width, int image_height,
                  int samples_per_pixel, int max_depth, const vec3 &background,
                  const camera &cam, const hittable &world,
                  std::vector<vec3> &image_buffer,
                  std::atomic<int> &rows_completed) {
  for (int j = start_y; j < end_y; ++j) {
    for (int i = 0; i < image_width; ++i) {
      color pixel_color(0, 0, 0);

      for (int s = 0; s < samples_per_pixel; ++s) {
        double u = (i + random_double()) / image_width;
        double v = (j + random_double()) / image_height;
        ray r = cam.get_ray(u, v);
        pixel_color += ray_color(r, background, world, max_depth);
      }

      image_buffer[j * image_width + i] = pixel_color;
    }
    rows_completed.fetch_add(1);
  }
}

int main() {
  const int image_width = 1280;
  const int image_height = 720;
  const int samples_per_pixel = 75;
  const int max_depth = 50;

  vec3 background(0.5, 0.7, 1.0);

  hittable_list world;

  // Floor
  auto checker = make_shared<checker_texture>(
      make_shared<solid_color>(vec3(0.2, 0.3, 0.1)),
      make_shared<solid_color>(vec3(0.9, 0.9, 0.9)));
  auto floor_mat = make_shared<lambertian>(checker);
  world.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000.0, floor_mat));

  // Main light
  auto light_mat =
      make_shared<diffuse_light>(make_shared<solid_color>(vec3(4, 4, 4)));
  world.add(make_shared<sphere>(point3(1.0, 2.0, -0.5), 0.75, light_mat));

  // Soft sky
  auto sky_mat = make_shared<diffuse_light>(
      make_shared<solid_color>(vec3(0.15, 0.22, 0.35)));
  world.add(make_shared<sphere>(point3(0, 100, 0), 90, sky_mat));

  // Earth
  auto earth_tex = make_shared<image_texture>("assets/earthmap.jpg");
  auto earth_mat = make_shared<lambertian>(earth_tex);
  world.add(make_shared<sphere>(point3(-2.0, 0.5, 0.0), 0.5, earth_mat));

  // Model
  auto model_mat = make_shared<metal>(color(0.8, 0.6, 0.2), 0.05);
  auto model = load_model("assets/monkeyhead.obj", model_mat);
  auto model_bvh = make_shared<bvh_node>(model.objects, 0, model.objects.size());
  world.add(
      make_shared<translate>(model_bvh, vec3(0.0, 0.985, 0.0)));

  // BVH

  bvh_node bvh_world(world.objects, 0, world.objects.size());

  // Camera
  camera cam(point3(5, 3.5, 5), point3(0, 0.3, 0), vec3(0, 1, 0), 40,
             1.0 * image_width / image_height);

  std::vector<vec3> image_buffer(image_width * image_height);
  std::atomic<int> rows_completed{0};

  unsigned int num_threads = std::thread::hardware_concurrency();
  if (num_threads == 0)
    num_threads = 8;

  std::vector<std::thread> threads;
  int rows_per_thread = image_height / num_threads;

  std::atomic<bool> rendering_done{false};
  std::thread monitor([&]() {
    while (!rendering_done) {
      int done = rows_completed.load();
      int pct = done * 100 / image_height;
      std::cerr << "\rRendering: " << pct << "% (" << done << "/"
                << image_height << " rows)" << std::flush;
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
  });

  for (unsigned int t = 0; t < num_threads; ++t) {
    int start_y = t * rows_per_thread;
    int end_y =
        (t == num_threads - 1) ? image_height : start_y + rows_per_thread;

    threads.push_back(std::thread(
        render_chunk, start_y, end_y, image_width, image_height,
        samples_per_pixel, max_depth, std::ref(background), std::ref(cam),
        std::ref(bvh_world), std::ref(image_buffer), std::ref(rows_completed)));
  }

  for (auto &t : threads) {
    t.join();
  }

  rendering_done = true;
  monitor.join();

  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  for (int j = image_height - 1; j >= 0; --j) {
    for (int i = 0; i < image_width; ++i) {
      write_color(std::cout, image_buffer[j * image_width + i],
                  samples_per_pixel);
    }
  }

  std::cerr << "\nDone.\n";
  return 0;
}
