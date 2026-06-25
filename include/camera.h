#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"
#include "rtweekend.h"

class camera {
public:
  camera(point3 lookfrom, point3 lookat, vec3 vup, double vfov,
         double aspect_ratio);

  ray get_ray(double u, double v) const;

private:
  point3 origin;
  point3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
};

#endif // !CAMERA_H
