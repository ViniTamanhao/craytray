#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "ray.h"
#include "rtweekend.h"
#include "texture.h"
#include "vec3.h"
#include <cmath>
#include <memory>

class material {
public:
  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       vec3 &attenuation, ray &scattered) const = 0;

  virtual vec3 emitted(double u, double v, const vec3 &p) const {
    return vec3(0, 0, 0);
  }
};

class lambertian : public material {
public:
  lambertian(shared_ptr<texture> a) : albedo(a) {}

  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       vec3 &attenuation, ray &scattered) const override;

private:
  shared_ptr<texture> albedo;
};

class metal : public material {
public:
  metal(const vec3 &a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       vec3 &attenuation, ray &scattered) const override;

private:
  vec3 albedo;
  double fuzz;
};

class dielectric : public material {
public:
  dielectric(double index_of_refraction) : ir(index_of_refraction) {}

  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       vec3 &attenuation, ray &scattered) const override;

private:
  double ir;

  static double reflectance(double cosine, double ref_idx) {
    double r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 + r0) * pow(1 - cosine, 5);
  }
};

class diffuse_light : public material {
public:
  diffuse_light(shared_ptr<texture> c) : color(c) {}

  virtual bool scatter(const ray &r_in, const hit_record &rec,
                       vec3 &attenuation, ray &scattered) const override;

  virtual vec3 emitted(double u, double v, const vec3 &p) const override {
    return color->value(u, v, p);
  }

private:
  shared_ptr<texture> color;
};

#endif // !MATERIAL_H
