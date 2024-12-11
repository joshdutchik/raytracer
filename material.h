#ifndef MATERIAL_H
#define MATERIAL_H

// header file for an object's material and helps calculate either the emitted color or scattered color
// depending on the material type

// include
#include "utility.h"
#include "texture.h"

// material
class material
{
public:
  // function to determine the color that is emitted off the object material
  virtual color emitted(double u, double v, const point3 &p) const
  {
    return color(0, 0, 0);
  }

  // function to determine if the ray is scattered off the object or not
  virtual bool scatter(const ray &r_in, const place_hit &rec, color &attenuation, ray &scattered) const
  {
    return false;
  }
};

// specular
class specular : public material
{
public:
  // constructor for specular material given a color and a "fuzz" value
  specular(const color &solid_c, double spec_fuzz) : solid_c(solid_c)
  {
    if (spec_fuzz > 1)
    {
      fuzz = 1;
    }

    else
    {
      fuzz = spec_fuzz;
    }
  }

  // function to determine if the ray will scatter
  bool scatter(const ray &r_in, const place_hit &rec, color &attenuation, ray &scattered) const override
  {
    vec3 vec_ref = reflect(r_in.direction(), rec.normal);

    vec_ref = unit_vector(vec_ref) + (fuzz * random_unit_vector());
    scattered = ray(rec.p, vec_ref, r_in.time());
    attenuation = solid_c;

    return (dot(scattered.direction(), rec.normal) > 0);
  }

private:
  double fuzz;
  color solid_c;
};

// diffuse
class diffuse : public material
{
public:
  // constructor to add a diffuse material with only a color
  diffuse(const color &solid_c) : tex(make_shared<solid_color>(solid_c)) {}

  // constructor to create a diffuse object with the given texture
  diffuse(shared_ptr<texture> tex) : tex(tex) {}

  // scatter function
  bool scatter(const ray &r_in, const place_hit &rec, color &attenuation, ray &scattered) const override
  {
    auto scatter_direction = rec.normal + random_unit_vector();

    if (scatter_direction.near_zero())
    {
      scatter_direction = rec.normal;
    }

    scattered = ray(rec.p, scatter_direction, r_in.time());
    attenuation = tex->value(rec.u, rec.v, rec.p);

    return true;
  }

private:
  shared_ptr<texture> tex;
};

// dielectric
class dielectric : public material
{
public:
  // constructor for a dielectric material on an object
  dielectric(double refraction_index) : refraction_index(refraction_index) {}

  // scatter function
  bool scatter(const ray &r_in, const place_hit &rec, color &attenuation, ray &scattered) const override
  {
    vec3 direction;
    attenuation = color(1.0, 1.0, 1.0);
    double ri = refraction_index;

    if (rec.front_face)
    {
      ri = 1.0 / refraction_index;
    }

    vec3 unit_direction = unit_vector(r_in.direction());

    double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool unrefractable = ri * sin_theta > 1.0;

    if (unrefractable || reflectance(cos_theta, ri) > random_double())
    {
      direction = reflect(unit_direction, rec.normal);
    }

    else
    {
      direction = refract(unit_direction, rec.normal, ri);
    }

    scattered = ray(rec.p, direction, r_in.time());

    return true;
  }

private:
  double refraction_index;

  // helper function to help calculate reflectance
  static double reflectance(double cosine, double refraction_index)
  {
    auto r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;

    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
  }
};

// emissive
class emissive : public material
{
public:
  // constructor for light material given a color
  emissive(const color &emit) : tex(make_shared<solid_color>(emit)) {}

  // emitted function
  color emitted(double u, double v, const point3 &p) const override
  {
    return tex->value(u, v, p);
  }

private:
  shared_ptr<texture> tex;
};

// volume material
class volume_mat : public material
{
public:
  // constructor for volume material with a solid color
  volume_mat(const color &solid_c) : tex(make_shared<solid_color>(solid_c)) {}

  // scatter function
  bool scatter(const ray &r_in, const place_hit &rec, color &attenuation, ray &scattered) const override
  {
    scattered = ray(rec.p, random_unit_vector(), r_in.time());
    attenuation = tex->value(rec.u, rec.v, rec.p);

    return true;
  }

private:
  shared_ptr<texture> tex;
};

#endif