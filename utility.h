#ifndef UTILITY_H
#define UTILITY_H

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
void clearLine() {
    std::cout << "\r" << std::string(50, ' ') << "\r";
}

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

// Returns a random real in [min,max).
inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

// Returns a random integer in [min,max].
inline int random_int(int min, int max) {
    return int(random_double(min, max+1));
}

// include
#include "vec3.h"
#include "interval.h"
#include "color.h"
#include "ray.h"
#include "texture_vector.h"
#include "debugger.h"
#include "hittable.h"
#include "perlin.h"

#endif