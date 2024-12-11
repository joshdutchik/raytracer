#ifndef HITTABLE_H
#define HITTABLE_H

// header file to define how an object should react when hit by a ray

// include
#include "utility.h"
#include "AA_bounding_box.h"

// material
class material;

// place_hit
class place_hit
{
public:
  double t, u, v;
  bool front_face;
  point3 p;
  vec3 normal;
  shared_ptr<material> mat;

  void set_face_normal(const ray &r, const vec3 &outward_normal)
  {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal : -outward_normal;
  }
};

// hittable
class hittable
{
public:
  virtual bool hit(const ray &r, interval ray_t, place_hit &rec) const = 0;

  virtual AA_bounding_box bounding_box() const = 0;
};

#endif