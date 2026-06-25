#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "ray.h"
#include "rtweekend.h"
#include "sphere.h"
#include "texture.h"
#include "vec3.h"
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

  vec3 emitted = rec.mat_ptr->emitted(0, 0, rec.p);
  ray scattered;
  vec3 attenuation;

  if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
    return emitted +
           attenuation * ray_color(scattered, background, world, depth - 1);

  return emitted;
}

void render_chunk(int start_y, int end_y, int image_width, int image_height,
                  int samples_per_pixel, int max_depth, const vec3 &background,
                  const camera &cam, const hittable_list &world,
                  std::vector<vec3> &image_buffer) {
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
  }
}

int main() {
  const int image_width = 400;
  const int image_height = 225;
  const int samples_per_pixel = 100;
  const int max_depth = 50;

  vec3 background(0, 0, 0);

  hittable_list world;

  auto checker = make_shared<checker_texture>(
      make_shared<solid_color>(vec3(0.2, 0.3, 0.1)),
      make_shared<solid_color>(vec3(0.9, 0.9, 0.9)));

  auto floor_mat = make_shared<lambertian>(checker);

  auto earth_tex = make_shared<image_texture>("earthmap.jpg");
  auto earth_mat = make_shared<lambertian>(earth_tex);

  auto light_mat =
      make_shared<diffuse_light>(make_shared<solid_color>(vec3(4, 4, 4)));

  world.add(make_shared<sphere>(point3(0.0, -1000.0, 0.0), 1000.0, floor_mat));
  world.add(make_shared<sphere>(point3(0.0, 0.0, 0.0), 0.5, earth_mat));
  world.add(make_shared<sphere>(point3(0.0, 0.5, -1.0), 0.25, light_mat));


  camera cam(point3(3, 3, 2), point3(0, 0, -1), vec3(0, 1, 0), 40,
             1.0 * image_width / image_height);

  std::vector<vec3> image_buffer(image_width * image_height);

  unsigned int num_threads = std::thread::hardware_concurrency();
  if (num_threads == 0)
    num_threads = 8;

  std::vector<std::thread> threads;
  int rows_per_thread = image_height / num_threads;

  for (unsigned int t = 0; t < num_threads; ++t) {
    int start_y = t * rows_per_thread;
    int end_y =
        (t == num_threads - 1) ? image_height : start_y + rows_per_thread;

    threads.push_back(std::thread(render_chunk, start_y, end_y, image_width,
                                  image_height, samples_per_pixel, max_depth,
                                  std::ref(background), std::ref(cam),
                                  std::ref(world), std::ref(image_buffer)));
  }

  for (auto &t : threads) {
    t.join();
  }

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
