#ifndef UTILITY_H
#define UTILITY_H

// header file for more basic utilites, includes, functions, for all header files in program

// C utilities
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <vector>

// C++ Std Usings
using std::make_shared;
using std::shared_ptr;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
void clearLine()
{
    std::cout << "\r" << std::string(50, ' ') << "\r";
}

inline double degrees_to_radians(double degrees)
{
    return degrees * pi / 180.0;
}

inline double random_double()
{
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;

    return distribution(generator);
}

inline double random_double(double min, double max)
{
    return min + (max - min) * random_double();
}

inline int random_int(int min, int max)
{
    return int(random_double(min, max + 1));
}

// interval utilites
// interval
class interval {
  public:
    double min, max;
    
    // default constructor to set min to infinity and max to -infinity
    interval() : min(+infinity), max(-infinity) {}

    // constructor for custom min and max
    interval(double min, double max) : min(min), max(max) {}

    // constructor for custom min intervals and max intervals
    interval(const interval& a, const interval& b) {
        min = a.min <= b.min ? a.min : b.min;
        max = a.max >= b.max ? a.max : b.max;
    }

    // helper interval functions
    double size() const {
        return max - min;
    }

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    interval expand(double delta) const {
        auto padding = delta/2;
        return interval(min - padding, max + padding);
    }

    static const interval empty, universe;
};

// interval variables
const interval interval::empty    = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);

// interval basic operations
interval operator+(const interval& ival, double displacement) {
    return interval(ival.min + displacement, ival.max + displacement);
}

interval operator+(double displacement, const interval& ival) {
    return ival + displacement;
}

// more of the basic in folder includes
#include "vec3.h"
#include "ray.h"
#include "texture_vector.h"
#include "debugger.h"
#include "hittable.h"
#include "perlin.h"

using color = vec3;

#endif