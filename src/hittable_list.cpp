#include "hittable_list.h"
#include "aabb.h"

void hittable_list::clear() { objects.clear(); }

void hittable_list::add(shared_ptr<hittable> object) {
  objects.push_back(object);
}

bool hittable_list::hit(const ray &r, double ray_tmin, double ray_tmax,
                        hit_record &rec) const {
  hit_record temp_rec;
  bool hit_anything = false;
  double closest_so_far = ray_tmax;

  for (const auto &object : objects) {
    if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      rec = temp_rec;
    }
  }

  return hit_anything;
}

bool hittable_list::bounding_box(double time0, double time1,
                                 aabb &output_box) const {
  if (objects.empty())
    return false;
  aabb temp_box;
  bool first_box = true;

  for (const auto &obj : objects) {
    if (!obj->bounding_box(time0, time1, temp_box))
      return false;
    output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
    first_box = false;
  }
  return true;
}
