#ifndef VOLUME_H
#define VOLUME_H

// header file for volume (smoke, fog) object

// include statements
#include "utility.h"
#include "material.h"

// volume class
class volume : public hittable
{
public:
  // constructor
  volume(shared_ptr<hittable> fitted_object, double dens = 0.01, const color &solid_c = color(0, 0, 0)) : fitted_object(fitted_object), volume_material(make_shared<volume_mat>(solid_c))
  {
    density = -1 / dens;
  }

  // function to detect if ray has intersect volume object
  bool intersect(const ray &r, interval ray_t, place_hit &rec) const override
  {
    place_hit rec1, rec2;

    // make sure volume object has been intersect, if no return false
    if (!fitted_object->intersect(r, interval::universe, rec1))
    {
      return false;
    }

    // make sure volume object has been intersect, if no return false
    if (!fitted_object->intersect(r, interval(rec1.t + 0.0001, infinity), rec2))
    {
      return false;
    }

    // if statements to calculate the scatter point of the volume, as density increases so does likelihood
    // set record to new min if intersect
    if (rec1.t < ray_t.min)
    {
      rec1.t = ray_t.min;
    }

    // set record to new max if intersect
    if (rec2.t > ray_t.max)
    {
      rec2.t = ray_t.max;
    }

    // check if density is increasing
    if (rec1.t >= rec2.t)
    {
      return false;
    }

    // make sure not negative before setting
    if (rec1.t < 0)
    {
      rec1.t = 0;
    }

    auto ray_length = r.direction().length();
    auto boudary_distance = (rec2.t - rec1.t) * ray_length;
    auto hit_distance = density * std::log(random_double());

    // make sure inside boundary
    if (hit_distance > boudary_distance)
    {
      return false;
    }

    // set the point of intersect variables
    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);
    rec.normal = vec3(1, 0, 0);
    rec.front_face = true;
    rec.mat = volume_material;

    return true;
  }

  // function to return the axis aligned bounding box
  AA_bounding_box bounding_box() const override
  {
    return fitted_object->bounding_box();
  }

private:
  // density of the volume object
  double density;
  // the shape of the volume object
  shared_ptr<hittable> fitted_object;
  // the volume material
  shared_ptr<material> volume_material;
};

#endif