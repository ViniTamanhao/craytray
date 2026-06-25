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

    if (std::fabs(det) < epsilon)
      return false;
    double inv_det = 1.0 / det;

    vec3 tvec = r.origin() - v0;
    double u = dot(tvec, pvec) * inv_det;
    if (u < 0.0 || u > 1.0)
      return false;

    vec3 qvec = cross(tvec, e1);
    double v = dot(r.direction(), qvec) * inv_det;
    if (v < 0.0 || u + v > 1.0)
      return false;

    double t = dot(e2, qvec) * inv_det;
    if (t < ray_tmin || t > ray_tmax)
      return false;

    rec.t = t;
    rec.p = r.at(t);
    rec.set_face_normal(r, normal);
    rec.mat_ptr = mat_ptr;

    return true;
  }

  virtual bool bounding_box(double /*time0*/, double /*time1*/,
                            aabb &output_box) const override {
    vec3 min_v(fmin(fmin(v0.x(), v0.x() + e1.x()), v0.x() + e2.x()),
               fmin(fmin(v0.y(), v0.y() + e1.y()), v0.y() + e2.y()),
               fmin(fmin(v0.z(), v0.z() + e1.z()), v0.z() + e2.z()));
    vec3 max_v(fmax(fmax(v0.x(), v0.x() + e1.x()), v0.x() + e2.x()),
               fmax(fmax(v0.y(), v0.y() + e1.y()), v0.y() + e2.y()),
               fmax(fmax(v0.z(), v0.z() + e1.z()), v0.z() + e2.z()));

    double eps = 1e-6;
    if (max_v.x() - min_v.x() < eps) {
      min_v.e[0] -= eps;
      max_v.e[0] += eps;
    }
    if (max_v.y() - min_v.y() < eps) {
      min_v.e[1] -= eps;
      max_v.e[1] += eps;
    }
    if (max_v.z() - min_v.z() < eps) {
      min_v.e[2] -= eps;
      max_v.e[2] += eps;
    }
    output_box = aabb(min_v, max_v);
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
