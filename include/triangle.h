#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "material.h"
#include "ray.h"
#include "rtweekend.h"
#include "vec3.h"
#include <cmath>

class triangle : public hittable {
public:
  triangle(const vec3 &v0, const vec3 &v1, const vec3 &v2,
           shared_ptr<material> m)
      : v0(v0), mat_ptr(m) {
    e1 = v1 - v0;
    e2 = v2 - v0;
    normal = unit_vector(cross(e1, e2));
  }

  bool hit(const ray &r, double ray_tmin, double ray_tmax,
           hit_record &rec) const override {
    double epsilon = 1e-8;

    vec3 pvec = cross(r.direction(), e2);
    double det = dot(e1, pvec);

    if (std::fabs(det) < epsilon) return false;
    double inv_det = 1.0 / det;

    vec3 tvec = r.origin() - v0;
    double u = dot(tvec, pvec) * inv_det;
    if (u < 0.0 || u > 1.0) return false;

    vec3 qvec = cross(tvec, e1);
    double v = dot(r.direction(), qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0) return false;

    double t = dot(e2, qvec) * inv_det;
    if (t < ray_tmin || t > ray_tmax) return false;

    rec.t = t;
    rec.p = r.at(t);
    rec.set_face_normal(r, normal);
    rec.mat_ptr = mat_ptr;

    return true;
  }

private:
  vec3 v0;
  vec3 e1;
  vec3 e2;
  vec3 normal;
  shared_ptr<material> mat_ptr;
};

#endif // !TRIANGLE_H
