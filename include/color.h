#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include <algorithm>
#include <cmath>
#include <ostream>

inline void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
  double scale = 1.0 / samples_per_pixel;
  double r = pixel_color.x() * scale;
  double g = pixel_color.y() * scale;
  double b = pixel_color.z() * scale;

  r = sqrt(r);
  g = sqrt(g);
  b = sqrt(b);

  int ir = int(256 * std::clamp(r, 0.0, 0.999));
  int ig = int(256 * std::clamp(g, 0.0, 0.999));
  int ib = int(256 * std::clamp(b, 0.0, 0.999));

  out << ir << ' ' << ig << ' ' << ib << '\n';
}

#endif // !COLOR_H

