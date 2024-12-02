#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "hittable.h"
#include "vec3.h"

class triangle : public hittable
{
public:
    // Constructor
    triangle(const point3 &point0, const point3 &point1, const point3 &point2, shared_ptr<material> material)
    {
        p0 = point0;
        p1 = point1;
        p2 = point2;
        mat = material;
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        
    }

    aabb bounding_box() const override { return bbox; }

private:
    point3 p0, p1, p2; // Triangle vertices
    shared_ptr<material> mat;
    aabb bbox;
};

#endif