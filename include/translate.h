#ifndef TRANSLATE_H
#define TRANSLATE_H

#include "hittable.h"
#include "vec3.h"

class translate : public hittable {
public:
  translate(shared_ptr<hittable> p, const vec3& displacement)
    : ptr(p), offset(displacement) {}

  bool hit(const ray& r, double ray_tmin, double ray_tmax,
           hit_record& rec) const override {
    ray moved_r(r.origin() - offset, r.direction());
    if (!ptr->hit(moved_r, ray_tmin, ray_tmax, rec))
      return false;

    rec.p += offset;
    return true;
  }

private:
  shared_ptr<hittable> ptr;
  vec3 offset;
};

#endif
