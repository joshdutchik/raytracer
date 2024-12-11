#ifndef RAY_H
#define RAY_H

// header file for the ray utility class

// include
#include "utility.h"

// ray
class ray
{
public:
  // constructor
  ray() {}

  // constructor with ray with given origin, direction and time
  ray(const point3 &origin, const vec3 &direction, double time) : ray_orig(origin), ray_dir(direction), ray_time(time) {}

  // constructor with given origin and direction
  ray(const point3 &origin, const vec3 &direction) : ray(origin, direction, 0) {}

  // helper ray functions
  const point3 &origin() const
  {
    return ray_orig;
  }

  const vec3 &direction() const
  {
    return ray_dir;
  }

  double time() const
  {
    return ray_time;
  }

  point3 at(double t) const
  {
    return ray_orig + t * ray_dir;
  }

private:
  point3 ray_orig;
  vec3 ray_dir;
  double ray_time;
};

#endif