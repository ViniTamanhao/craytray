#ifndef SPHERE_H
#define SPHERE_H

#include "aabb.h"
#include "hittable.h"
#include "material.h"
#include "rtweekend.h"
#include "vec3.h"
#include <cmath>

class sphere : public hittable {
public:
  sphere() {}
  sphere(point3 cen, double r, shared_ptr<material> m)
      : center(cen), radius(r), mat_ptr(m) {}

  bool hit(const ray &r, double ray_tmin, double ray_tmax,
           hit_record &rec) const override;

  virtual bool bounding_box(double time0, double tim1,
                            aabb &output_box) const override;

private:
  point3 center;
  double radius;
  shared_ptr<material> mat_ptr;

  static void get_sphere_uv(const vec3 &p, double &u, double &v) {
    // p: a unit vector from sphere center to hit point
    double theta = acos(-p.y());
    double phi = atan2(-p.z(), p.x()) + pi;
    u = phi / (2 * pi);
    v = theta / pi;
  }
};

#endif // !SPHERE_H
