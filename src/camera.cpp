#include "camera.h"
#include <cmath>

camera::camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov,
               double aspect_ratio) {
  double theta = degrees_to_radius(vfov);
  double h = tan(theta / 2);
  double viewport_height = 2.0 * h;
  double viewport_width = aspect_ratio * viewport_height;

  w = unit_vector(lookfrom - lookat);
  u = unit_vector(cross(vup, w));
  v = cross(w, u);

  origin = lookfrom;
  horizontal = viewport_width * u;
  vertical = viewport_height * v;
  lower_left_corner = origin - horizontal / 2 - vertical / 2 - w;
}

ray camera::get_ray(double u_ratio, double v_ratio) const {
  return ray(origin,
             lower_left_corner + u_ratio * horizontal + v_ratio * vertical -
                 origin);
}
