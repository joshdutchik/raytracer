#ifndef PERLIN_H
#define PERLIN_H

// header file for perlin noise creation

// include
#include "utility.h"

// perlin
class perlin
{
public:
    // constructor for the perlin texture
    perlin()
    {
        for (int i = 0; i < number_of_points; i++)
        {
            randvec[i] = unit_vector(vec3::random(-1, 1));
        }

        generate_permute(perm_x);
        generate_permute(perm_y);
        generate_permute(perm_z);
    }

    // function to create the noise
    double generate_noise(const point3 &p) const
    {
        auto u = p.x() - std::floor(p.x());
        auto v = p.y() - std::floor(p.y());
        auto w = p.z() - std::floor(p.z());

        auto i = int(std::floor(p.x()));
        auto j = int(std::floor(p.y()));
        auto k = int(std::floor(p.z()));
        vec3 c[2][2][2];

        for (int di = 0; di < 2; di++)
        {
            for (int dj = 0; dj < 2; dj++)
            {
                for (int dk = 0; dk < 2; dk++)
                {
                    c[di][dj][dk] = randvec[perm_x[(i + di) & 255] ^ perm_y[(j + dj) & 255] ^ perm_z[(k + dk) & 255]];
                }
            }
        }

        return interpolation(c, u, v, w);
    }

    // function to calculate turbulance
    double create_turbulence(const point3 &p, int depth) const
    {
        auto accum = 0.0;
        auto temp_p = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; i++)
        {
            accum += weight * generate_noise(temp_p);
            weight *= 0.5;
            temp_p *= 2;
        }

        return std::fabs(accum);
    }

private:
    static const int number_of_points = 256;
    int perm_x[number_of_points], perm_y[number_of_points], perm_z[number_of_points];
    double randfloat[number_of_points];
    vec3 randvec[number_of_points];

    // function to generate permute
    static void generate_permute(int *p)
    {
        for (int i = 0; i < number_of_points; i++)
            p[i] = i;

        permute(p, number_of_points);
    }

    // helper function to actually permute
    static void permute(int *p, int n)
    {
        for (int i = n - 1; i > 0; i--)
        {
            int target = random_int(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    // interpolation function
    static double interpolation(const vec3 c[2][2][2], double u, double v, double w)
    {
        auto uu = u * u * (3 - 2 * u);
        auto vv = v * v * (3 - 2 * v);
        auto ww = w * w * (3 - 2 * w);
        auto accum = 0.0;

        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                for (int k = 0; k < 2; k++)
                {
                    vec3 weight_v(u - i, v - j, w - k);
                    accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv)) * (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight_v);
                }
            }
        }

        return accum;
    }
};

#endif