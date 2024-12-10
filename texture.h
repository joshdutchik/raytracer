#ifndef TEXTURE_H
#define TEXTURE_H

#include "utility.h"
#include "rtw_stb_image.h"

class texture
{

public:
  virtual color value(double u, double v, const point3 &p) const = 0;
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

class sunset : public texture
{
public:
  sunset(double max, double min) : max(max), min(min) {}

  color value(double u, double v, const point3 &p) const override
  {
    // Normalize theta to range [0, 1]
    double r, g, b;
    double t = (p.y() - min) / (max - min);

    // Gradient colors
    if (t < 0.5)
    {
      // Bottom to middle (red to pink)
      r = 1.0;
      g = t * 2.0; // Interpolates green from 0 to 1
      b = t * 0.8; // Interpolates blue from 0 to 0.8
    }

    else
    {
      // Middle to top (pink to dark blue)
      t = (t - 0.5) * 2.0; // Re-normalize t to [0, 1]
      r = 1.0 - t;         // Red fades out
      g = 1.0 - t * 0.7;   // Green fades to 0.3
      b = 0.8 + t * 0.2;   // Blue intensifies
    }

    return color(r, g, b);
  }

private:
  double max, min;
};

class rainbow : public texture
{
public:
  rainbow(double max, double min) : max(max), min(min) {}

  color value(double u, double v, const point3 &p) const override
  {
    double boundary = (max-min) / 7;

    int rainbow_color_value = 6;

    for (int i = 1; i <= 7; i++)
    {
      if (p.y() < ((boundary * i) + min))
      {
        rainbow_color_value = i - 1;
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
      return color(2.55, 0, 2.55);
    }
  }

private:
  double max, min;
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

class hashed : public texture
{
public:
  hashed(color scaled) : scaled(scaled)
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

    return scaled * randfloat[hash_x[i] ^ hash_y[j] ^ hash_z[k]];
  }

private:
  color scaled;

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

class perlin_noise : public texture
{
public:
  perlin_noise(double scale, color scaled) : scale(scale), scaled(scaled) {}

  color value(double u, double v, const point3 &p) const override
  {
    return scaled * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
  }

private:
  perlin noise;
  double scale;
  color scaled;
};

#endif