#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"
#include "vec3.h"
#include <algorithm>
#include <cmath>
#include <iostream>
#include "stb_image.h"

class texture {
public:
  virtual ~texture() = default;
  virtual vec3 value(double u, double v, const vec3 &p) const = 0;
};

class solid_color : public texture {
public:
  solid_color() {}
  solid_color(const vec3 &c) : color_value(c) {}
  solid_color(double r, double g, double b) : solid_color(vec3(r, g, b)) {}

  vec3 value(double u, double v, const vec3 &p) const override {
    return color_value;
  }

private:
  vec3 color_value;
};

class checker_texture : public texture {
public:
  checker_texture(shared_ptr<texture> even, shared_ptr<texture> odd)
    : even(even), odd(odd) {}

  vec3 value(double u, double v, const vec3 &p) const override {
    double sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    return sines < 0 ? odd->value(u, v, p): even->value(u, v, p);
  }

private:
  shared_ptr<texture> even;
  shared_ptr<texture> odd;
};

class image_texture : public texture {
public:
  image_texture(const char* filename) {
    data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) {
      std::cerr << "ERROR: Could not load texture image: " << filename << "\n";
      width = height = 0;
    }
  }

  ~image_texture() {
    if (data) stbi_image_free(data);
  }

  vec3 value(double u, double v, const vec3& p) const override {
    if (data == nullptr)
      return vec3(0, 1, 1);

    u = std::clamp(u, 0.0, 1.0);
    v = 1.0 - std::clamp(v, 0.0, 1.0);

    int i = static_cast<int>(u * width);
    int j = static_cast<int>(v * height);
    i = std::clamp(i, 0, width - 1);
    j = std::clamp(j, 0, height - 1);

    int idx = (j * width + i) * channels;
    double r = data[idx]     / 255.0;
    double g = data[idx + 1] / 255.0;
    double b = data[idx + 2] / 255.0;

    return vec3(r, g, b);
  }

private:
  unsigned char* data = nullptr;
  int width = 0, height = 0, channels = 0;
};

#endif // !TEXTURE_H
