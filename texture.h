#ifndef TEXTURE_H
#define TEXTURE_H

// header for the texture in material

// include
#include "utility.h"
#include "image_loader.h"

// texture
class texture
{
public:
  virtual color value(double u, double v, const point3 &p) const = 0;
};

// solid color texture
class solid_color : public texture
{
public:
  // constructor for with color class
  solid_color(const color &solid_c) : solid_c(solid_c) {}

  // constructor for with rgb values
  solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

  // function to return color
  color value(double u, double v, const point3 &p) const override
  {
    return solid_c;
  }

private:
  color solid_c;
};

// sunset texture
class sunset : public texture
{
public:
  // constructor for sunset with max and min height of the object
  sunset(double max, double min) : max(max), min(min) {}

  // get color value
  color value(double u, double v, const point3 &p) const override
  {
    // normalize the y value between 0,1
    double r, g, b;
    double t = (p.y() - min) / (max - min);

    // if bottom half of the shape
    if (t < 0.5)
    {
      // red to pink
      r = 1.0;
      g = t * 2.0;
      b = t * 0.8;
    }

    else
    {
      // pink to dark blue
      t = (t - 0.5) * 2.0;
      r = 1.0 - t;
      g = 1.0 - t * 0.7;
      b = 0.8 + t * 0.2;
    }

    return color(r, g, b);
  }

private:
  double max, min;
};

// rainbow texture
class rainbow : public texture
{
public:
  // constructor for rainbow using max and min height of the shapes
  rainbow(double max, double min) : max(max), min(min) {}

  // return the color value
  color value(double u, double v, const point3 &p) const override
  {
    // split up color values into seven groups
    double boundary = (max - min) / 7;
    int rainbow_color_value = 6;

    // sort y values into the right group
    for (int i = 1; i <= 7; i++)
    {
      if (p.y() < ((boundary * i) + min))
      {
        rainbow_color_value = i - 1;
        break;
      }
    }

    // apply the correct color
    switch (rainbow_color_value)
    {
    case 0:
      // violet
      return color(1.48, 0, 2.11);
    case 1:
      // indigo
      return color(.75, 0, 1.3);
    case 2:
      // blue
      return color(0, 0, 2.55);
    case 3:
      // green
      return color(0, 2.55, 0);
    case 4:
      // yellow
      return color(2.55, 2.55, 0);
    case 5:
      // orange
      return color(2.55, 1.27, 0);
    case 6:
      // reds
      return color(2.55, 0, 0);
    default:
      // gray
      return color(2.55, 0, 2.55);
    }
  }

private:
  double max, min;
};

// custom image texture
class image_texture : public texture
{
public:
  // constructor for taking a file name and using it as the  texture
  image_texture(const char *filename) : image(filename) {}

  // return the color value
  color value(double u, double v, const point3 &p) const override
  {
    // calculate correct color value using the image loader
    u = interval(0, 1).clamp(u);
    v = 1.0 - interval(0, 1).clamp(v);

    auto i = int(u * image.width());
    auto j = int(v * image.height());

    auto pixel = image.pixel_data(i, j);
    auto color_scale = 1.0 / 255.0;

    return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
  }

private:
  image_loader image;
};

// hashed texture
class hashed : public texture
{
public:
  // constructor for hashed texture with given color
  hashed(color solid_c) : solid_c(solid_c)
  {
    for (int i = 0; i < point_count; i++)
    {
      randfloat[i] = random_double();
    }

    // generate hashes
    generate_hash(hash_x);
    generate_hash(hash_y);
    generate_hash(hash_z);
  }

  // return the color value
  color value(double u, double v, const point3 &p) const override
  {
    // box each pixel value into boxes
    auto i = int(4 * p.x()) & 255;
    auto j = int(4 * p.y()) & 255;
    auto k = int(4 * p.z()) & 255;

    // return solid_c color value
    return solid_c * randfloat[hash_x[i] ^ hash_y[j] ^ hash_z[k]];
  }

private:
  color solid_c;

  static const int point_count = 256;
  double randfloat[point_count];

  int hash_x[point_count];
  int hash_y[point_count];
  int hash_z[point_count];

  // helper function to help generate hash
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

// perlin noise texture
class perlin_noise : public texture
{
public:
  // constructor for perline noise given a scale and a color to solid_c
  perlin_noise(double scale, color solid_c) : scale(scale), solid_c(solid_c) {}

  // return the color value
  color value(double u, double v, const point3 &p) const override
  {
    return solid_c * (1 + std::sin(scale * p.z() + 10 * noise.create_turbulence(p, 7)));
  }

private:
  double scale;
  perlin noise;
  color solid_c;
};

#endif