#ifndef BVH_NODE_H
#define BVH_NODE_H

#include "aabb.h"
#include "hittable.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

class bvh_node : public hittable {
public:
  bvh_node() {}
  bvh_node(std::vector<shared_ptr<hittable>>& src_objects, size_t start, size_t end) {
    // Choose random axis
    int axis = int(random_double() * 3);

    auto comparator = (axis == 0) ? box_x_compare
                    : (axis == 1) ? box_y_compare
                                  : box_z_compare;

    size_t object_span = end - start;

    if (object_span == 1) {
      left = right = src_objects[start];
    } else if (object_span == 2) {
      left = src_objects[start];
      right = src_objects[start + 1];
    } else {
      std::sort(src_objects.begin() + start, src_objects.begin() + end, comparator);
      size_t mid = start + object_span / 2;
      left = make_shared<bvh_node>(src_objects, start, mid);
      right = make_shared<bvh_node>(src_objects, mid, end);
    }

    aabb left_box, right_box;
    if (!left->bounding_box(0, 0, left_box) || !right->bounding_box(0, 0, right_box))
      std::cerr << "No bounding box in BVH node constructor.\n";
    box = surrounding_box(left_box, right_box);
  }

  virtual bool hit(const ray& r, double ray_tmin, double ray_tmax,
                   hit_record& rec) const override {
    if (!box.hit(r, ray_tmin, ray_tmax)) return false;

    hit_record left_rec, right_rec;
    bool hit_left  = left->hit(r, ray_tmin, ray_tmax, left_rec);
    bool hit_right = right->hit(r, ray_tmin, ray_tmax, right_rec);

    if (!hit_left && !hit_right) return false;

    if (hit_left && (!hit_right || left_rec.t < right_rec.t)) {
      rec = left_rec;
    } else {
      rec = right_rec;
    }
    return true;
  }

  virtual bool bounding_box(double /*time0*/, double /*time1*/, aabb& output_box) const override {
    output_box = box;
    return true;
  }

private:
  shared_ptr<hittable> left;
  shared_ptr<hittable> right;
  aabb box;

  static bool box_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b, int axis) {
    aabb box_a, box_b;
    if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
      std::cerr << "No bounding box in BVH node constructor.\n";
    return box_a.minimum[axis] < box_b.minimum[axis];
  }

  static bool box_x_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b) {
    return box_compare(a, b, 0);
  }
  static bool box_y_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b) {
    return box_compare(a, b, 1);
  }
  static bool box_z_compare(const shared_ptr<hittable>& a, const shared_ptr<hittable>& b) {
    return box_compare(a, b, 2);
  }
};

#endif
