#include "sphere.h"
#include "aabb.h"

bool sphere::hit(const ray &r, double ray_tmin, double ray_tmax,
                 hit_record &rec) const {
  vec3 oc = r.origin() - center;
  double a = dot(r.direction(), r.direction());
  double b = 2.0 * dot(oc, r.direction());
  double c = dot(oc, oc) - radius * radius;
  double discriminant = b * b - 4 * a * c;
  if (discriminant < 0)
    return false;

  double sqrt_d = sqrt(discriminant);

  double t = (-b - sqrt_d) / (2.0 * a);
  if (t < ray_tmin || t > ray_tmax) {
    t = (-b + sqrt_d) / (2.0 * a);
    if (t < ray_tmin || t > ray_tmax)
      return false;
  }

  rec.t = t;
  rec.p = r.at(t);
  vec3 outward_normal = (rec.p - center) / radius;
  get_sphere_uv(outward_normal, rec.u, rec.v);
  rec.set_face_normal(r, outward_normal);
  rec.mat_ptr = mat_ptr;

  return true;
}

bool sphere::bounding_box(double /*time0*/, double /*time1*/,
                          aabb &output_box) const {
  output_box = aabb(center - vec3(radius, radius, radius),
                    center + vec3(radius, radius, radius));

  return true;
}
