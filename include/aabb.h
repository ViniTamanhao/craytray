#ifndef AABB_H
#define AABB_H

#include "ray.h"
#include "vec3.h"
#include <cmath>

class aabb {
public:
  point3 minimum;
  point3 maximum;

  aabb() {}
  aabb(const point3 &a, const point3 &b) : minimum(a), maximum(b) {}

  bool hit(const ray &r, double t_min, double t_max) const {
    double t0x = fmin((minimum.x() - r.origin().x()) / r.direction().x(),
                      (maximum.x() - r.origin().x()) / r.direction().x());

    double t1x = fmax((minimum.x() - r.origin().x()) / r.direction().x(),
                      (maximum.x() - r.origin().x()) / r.direction().x());

    t_min = fmax(t0x, t_min);
    t_max = fmin(t1x, t_max);
    if (t_max <= t_min)
      return false;

    double t0y = fmin((minimum.y() - r.origin().y()) / r.direction().y(),
                      (maximum.y() - r.origin().y()) / r.direction().y());
    double t1y = fmax((minimum.y() - r.origin().y()) / r.direction().y(),
                      (maximum.y() - r.origin().y()) / r.direction().y());

    t_min = fmax(t0y, t_min);
    t_max = fmin(t1y, t_max);
    if (t_max <= t_min)
      return false;

    double t0z = fmin((minimum.z() - r.origin().z()) / r.direction().z(),
                      (maximum.z() - r.origin().z()) / r.direction().z());
    double t1z = fmax((minimum.z() - r.origin().z()) / r.direction().z(),
                      (maximum.z() - r.origin().z()) / r.direction().z());
    t_min = fmax(t0z, t_min);
    t_max = fmin(t1z, t_max);
    if (t_max <= t_min)
      return false;

    return true;
  }
};

inline aabb surrounding_box(aabb box0, aabb box1) {
  point3 small(fmin(box0.minimum.x(), box1.minimum.x()),
               fmin(box0.minimum.y(), box1.minimum.y()),
               fmin(box0.minimum.z(), box1.minimum.z()));
  point3 big(fmax(box0.maximum.x(), box1.maximum.x()),
             fmax(box0.maximum.y(), box1.maximum.y()),
             fmax(box0.maximum.z(), box1.maximum.z()));
  return aabb(small, big);
}

#endif // !AABB_H
