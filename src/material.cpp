#include "material.h"

bool lambertian::scatter(const ray &r_in, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const {
  vec3 scatter_dir = rec.normal + random_unit_vector();
  if (scatter_dir.near_zero())
    scatter_dir = rec.normal;
  scattered = ray(rec.p, scatter_dir);
  attenuation = albedo;
  return true;
}

bool metal::scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation,
                    ray &scattered) const {
  vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
  scattered = ray(rec.p, reflected + fuzz * random_in_unit_sphere());
  attenuation = albedo;
  return (dot(scattered.direction(), rec.normal) > 0);
}

bool dielectric::scatter(const ray &r_in, const hit_record &rec,
                         vec3 &attenuation, ray &scattered) const {
  attenuation = color(1.0, 1.0, 1.0);
  double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

  vec3 unit_dir = unit_vector(r_in.direction());
  double cos_theta = fmin(dot(-unit_dir, rec.normal), 1.0);
  double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

  bool cannot_refract = refraction_ratio * sin_theta > 1.0;
  vec3 direction;

  if (cannot_refract ||
      reflectance(cos_theta, refraction_ratio) > random_double())
    direction = reflect(unit_dir, rec.normal);
  else
    direction = refract(unit_dir, rec.normal, refraction_ratio);

  scattered = ray(rec.p, direction);
  return true;
}

bool diffuse_light::scatter(const ray &r_in, const hit_record &rec,
                            vec3 &attenuation, ray &scattered) const {
  return false;
}
