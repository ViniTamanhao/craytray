#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <random>

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radius(double degrees) {
  return degrees * pi / 180.0;
}

inline double random_double() {
  static thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static thread_local std::mt19937 generator;
  return distribution(generator);
}

inline double random_double(double min, double max) {
  return min + (max - min) * random_double();
}

#endif // !RTWEEKEND_H

