#ifndef SPHERE_H
#define SPHERE_H

// header file for creating sphere objects both normal and moving

// include
#include "utility.h"

// sphere
class sphere : public hittable
{
public:
  // constructor for sphere with given center, radius, and material
  sphere(const point3 &center, double radius, shared_ptr<material> mat) : center(center, vec3(0, 0, 0)), radius(std::fmax(0, radius)), mat(mat)
  {
    // for debugging
    if (false)
    {
      std::stringstream outFile;
      outFile << "sphere: radius: " << radius << "\n";
      outFile << "sphere: center: (X: " << center.x() << ", Y: " << center.y() << ", Z: " << center.z() << "\n";
      outFile << "\n";

      debugger::getInstance().logToFile(outFile.str());
    }

    auto rvec = vec3(radius, radius, radius);
    aa_bound_box = AA_bounding_box(center - rvec, center + rvec);
  }

  // constructor for moving sphere with given center to center2, radius and material
  sphere(const point3 &center1, const point3 &center2, double radius, shared_ptr<material> mat) : center(center1, center2 - center1), radius(std::fmax(0, radius)), mat(mat)
  {
    // for debugging
    if (false)
    {
      std::stringstream outFile;
      outFile << "sphere: radius: " << radius << "\n";
      outFile << "sphere: center1: (X: " << center1.x() << ", Y: " << center1.y() << ", Z: " << center1.z() << "\n";
      outFile << "sphere: center2: (X: " << center2.x() << ", Y: " << center2.y() << ", Z: " << center2.z() << "\n";
      outFile << "\n";

      debugger::getInstance().logToFile(outFile.str());
    }

    auto rvec = vec3(radius, radius, radius);

    AA_bounding_box box1(center.at(0) - rvec, center.at(0) + rvec);
    AA_bounding_box box2(center.at(1) - rvec, center.at(1) + rvec);

    aa_bound_box = AA_bounding_box(box1, box2);
  }

  // function to determine if ray has intersect the sphere
  bool intersect(const ray &r, interval ray_t, place_hit &rec) const override
  {
    point3 current_center = center.at(r.time());
    vec3 oc = current_center - r.origin();

    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius * radius;

    auto discriminant = h * h - a * c;

    if (discriminant < 0)
    {
      return false;
    }

    auto sqrtd = std::sqrt(discriminant);
    auto root = (h - sqrtd) / a;

    if (!ray_t.surrounds(root))
    {
      root = (h + sqrtd) / a;

      if (!ray_t.surrounds(root))
      {
        return false;
      }
    }

    // place intersect variables for further calculations
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - current_center) / radius;
    rec.set_face_normal(r, outward_normal);
    get_sphere_uv(outward_normal, rec.u, rec.v);
    rec.mat = mat;

    return true;
  }

  AA_bounding_box bounding_box() const override { return aa_bound_box; }

private:
  double radius;
  ray center;
  shared_ptr<material> mat;
  AA_bounding_box aa_bound_box;

  // sphere vector function
  static void get_sphere_uv(const point3 &p, double &u, double &v)
  {
    auto theta = std::acos(-p.y());
    auto phi = std::atan2(-p.z(), p.x()) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
  }
};

#endif