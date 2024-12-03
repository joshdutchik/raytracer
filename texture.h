#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtweekend.h"
#include "perlin.h"
#include "rtw_stb_image.h"

class texture
{

public:
  virtual ~texture() = default;

  virtual color value(double u, double v, const point3 &p) const = 0;
};

class image_texture : public texture
{
public:
  image_texture(const char *filename) : image(filename) {}

  color value(double u, double v, const point3 &p) const override
  {
    // debug with pink color
    if (image.height() <= 0)
    {
      return color(2.55, 2.09, 2.2);
    }

    u = interval(0, 1).clamp(u);
    v = 1.0 - interval(0, 1).clamp(v);

    auto i = int(u * image.width());
    auto j = int(v * image.height());
    auto pixel = image.pixel_data(i, j);
    auto color_scale = 1.0 / 255.0;

    return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
  }

private:
  rtw_image image;
};

class gradient : public texture
{
  gradient() {}

  color value(double u, double v, const point3 &p) const override
  {
    double radius = 8;

    double normalized = (p.y() - 0) / (radius * 2);

    double red = 1 - normalized;
    double blue = normalized;

    return color(2.55 * red, 0, 2.55 * blue);
  }
};

class rainbow : public texture
{
public:
  rainbow() {}

  color value(double u, double v, const point3 &p) const override
  {
    /*
    int number_of_bands = 7;

    std::vector<float> boundaries(number_of_bands);

    // Surface area of each section: (4*pi*r^2) / numBands
    float totalArea = 4 * pi; // Total surface area of the sphere
    float areaPerBand = totalArea / number_of_bands;

    // We calculate the cumulative surface area and find the corresponding theta values
    float cumulativeArea = 0.0f;

    for (int i = 0; i < number_of_bands; i++)
    {
      float targetArea = areaPerBand * (i + 1);
      float theta = std::acos(1 - targetArea / (2 * pi));
      boundaries[i] = theta;
    }

    float r = std::sqrt(p.x() * p.x() + p.y() * p.y() + p.z() * p.z());
    float theta = std::acos(p.z() / r); // Latitude angle

    int rainbow_color_value = -1;

    if (theta < boundaries[0])
    {
      rainbow_color_value = 0;
    }

    else
    {
      for (int i = 0; i < boundaries.size()-1; i++)
      {
        if (theta >= boundaries[i] && theta < boundaries[i + 1])
        {
          rainbow_color_value = i + 1;
          break;
        }
      }
    }
    */

   double radius = 8;
   double boundary = (radius*2) / 7;
   int rainbow_color_value = 6;

   for (int i = 1; i <= 7; i++) {
      if (p.y() < boundary * i) {
        rainbow_color_value = i-1;
        break;
      }
   }

    switch (rainbow_color_value)
    {
    case 0:
      // red
      return color(2.55, 0, 0);
    case 1:
      // orange
      return color(2.55, 1.27, 0);
    case 2:
      // yellow
      return color(2.55, 2.55, 0);
    case 3:
      // green
      return color(0, 2.55, 0);
    case 4:
      // blue
      return color(0, 0, 2.55);
    case 5:
      // indigo
      return color(.75, 0, 1.3);
    case 6:
      // violet
      return color(1.48, 0, 2.11);
    default:
      // gray
      return color(1, 1, 1);
    }
  }
};

class hashed : public texture
{
public:
  hashed()
  {
    for (int i = 0; i < point_count; i++)
    {
      randfloat[i] = random_double();
    }

    generate_hash(hash_x);
    generate_hash(hash_y);
    generate_hash(hash_z);
  }

  color value(double u, double v, const point3 &p) const override
  {
    auto i = int(4 * p.x()) & 255;
    auto j = int(4 * p.y()) & 255;
    auto k = int(4 * p.z()) & 255;

    return color(.94, 1.57, .52) * randfloat[hash_x[i] ^ hash_y[j] ^ hash_z[k]];
  }

private:
  static const int point_count = 256;
  double randfloat[point_count];

  int hash_x[point_count];
  int hash_y[point_count];
  int hash_z[point_count];

  static void generate_hash(int *p)
  {
    for (int i = 0; i < point_count; i++)
    {
      p[i] = i;
    }

    for (int j = point_count - 1; j > 0; j--)
    {
      int target = random_int(0, j);
      int tmp = p[j];
      p[j] = p[target];
      p[target] = tmp;
    }
  }
};

class solid_color : public texture
{
public:
  solid_color(const color &albedo) : albedo(albedo) {}
  solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

  color value(double u, double v, const point3 &p) const override
  {
    return albedo;
  }

private:
  color albedo;
};

class checker_texture : public texture
{
public:
  checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
      : inv_scale(1.0 / scale), even(even), odd(odd) {}

  checker_texture(double scale, const color &c1, const color &c2)
      : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {}

  color value(double u, double v, const point3 &p) const override
  {
    auto xInteger = int(std::floor(inv_scale * p.x()));
    auto yInteger = int(std::floor(inv_scale * p.y()));
    auto zInteger = int(std::floor(inv_scale * p.z()));

    bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

    return isEven ? even->value(u, v, p) : odd->value(u, v, p);
  }

private:
  double inv_scale;
  shared_ptr<texture> even;
  shared_ptr<texture> odd;
};

class noise_texture : public texture
{
public:
  noise_texture(double scale) : scale(scale) {}

  color value(double u, double v, const point3 &p) const override
  {
    return color(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
  }

private:
  perlin noise;
  double scale;
};

#endif