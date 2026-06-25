#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"

class material;

struct hit_record {
  point3 p;
  vec3 normal;
  double t;
  double u = 0.0;
  double v = 0.0;
  bool front_face;
  shared_ptr<material> mat_ptr;

  void set_face_normal(const ray &r, const vec3 &outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
public:
  virtual bool hit(const ray &r, double ray_tmin, double ray_tmax,
                   hit_record &rec) const = 0;
};

#endif
