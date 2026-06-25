#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "material.h"

class sphere : public hittable {
public:
  sphere() {}
  sphere(point3 cen, double r, shared_ptr<material> m)
      : center(cen), radius(r), mat_ptr(m) {}

  bool hit(const ray &r, double ray_tmin, double ray_tmax,
           hit_record &rec) const override;

private:
  point3 center;
  double radius;
  shared_ptr<material> mat_ptr;
};

#endif // !SPHERE_H
