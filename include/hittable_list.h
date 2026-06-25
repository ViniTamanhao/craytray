#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "aabb.h"
#include "hittable.h"
#include <vector>

class hittable_list : public hittable {
public:
  hittable_list() {}
  hittable_list(shared_ptr<hittable> object) { add(object); }

  void clear();
  void add(shared_ptr<hittable> object);

  bool hit(const ray &r, double ray_tmin, double ray_tmax,
           hit_record &rec) const override;

  virtual bool bounding_box(double time0, double time1,
                            aabb &output_box) const override;

  std::vector<shared_ptr<hittable>> objects;
};

#endif // !HITTABLE_LIST_H
