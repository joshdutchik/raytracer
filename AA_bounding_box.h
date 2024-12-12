#ifndef AA_BOUNDING_BOX_H
#define AA_BOUNDING_BOX_H

// header file for the axis-aligned bounding box

// include
#include "utility.h"

// AA_bounding_box
class AA_bounding_box
{
public:
    interval x, y, z;

    // default constructor
    AA_bounding_box() {}

    // constructor with interval
    AA_bounding_box(const interval &x, const interval &y, const interval &z)
        : x(x), y(y), z(z)
    {
        pad_to_minimums();
    }

    // constructor with points
    AA_bounding_box(const point3 &a, const point3 &b)
    {
        x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);

        pad_to_minimums();
    }

    // constructor with given two other bounding boxes
    AA_bounding_box(const AA_bounding_box &box0, const AA_bounding_box &box1)
    {
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    // return the axis interval
    const interval &axis_interval(int n) const
    {
        if (n == 1)
        {
            return y;
        }

        if (n == 2)
        {
            return z;
        }

        return x;
    }

    // function to see if bounding box has been intersect
    bool intersect(const ray &r, interval ray_t) const
    {
        const point3 &ray_orig = r.origin();
        const vec3 &ray_dir = r.direction();

        for (int axis = 0; axis < 3; axis++)
        {
            const interval &ax = axis_interval(axis);
            const double adinv = 1.0 / ray_dir[axis];

            auto t0 = (ax.min - ray_orig[axis]) * adinv;
            auto t1 = (ax.max - ray_orig[axis]) * adinv;

            if (t0 < t1)
            {
                if (t0 > ray_t.min)
                {
                    ray_t.min = t0;
                }

                if (t1 < ray_t.max)
                {
                    ray_t.max = t1;
                }
            }

            else
            {
                if (t1 > ray_t.min)
                {
                    ray_t.min = t1;
                }

                if (t0 < ray_t.max)
                {
                    ray_t.max = t0;
                }
            }

            if (ray_t.max <= ray_t.min)
            {
                return false;
            }
        }
        return true;
    }

    // gets the longest axis of bounding box
    int longest_axis() const
    {
        if (x.size() > y.size())
        {
            return x.size() > z.size() ? 0 : 2;
        }

        else
        {
            return y.size() > z.size() ? 1 : 2;
        }
    }

    static const AA_bounding_box empty, universe;

private:
    // function to help pad to the minimums
    void pad_to_minimums()
    {
        double delta = 0.0001;

        if (x.size() < delta)
        {
            x = x.expand(delta);
        }

        if (y.size() < delta)
        {
            y = y.expand(delta);
        }

        if (z.size() < delta)
        {
            z = z.expand(delta);
        }
    }
};

// basic AA_bounding_box definitions
const AA_bounding_box AA_bounding_box::empty = AA_bounding_box(interval::empty, interval::empty, interval::empty);
const AA_bounding_box AA_bounding_box::universe = AA_bounding_box(interval::universe, interval::universe, interval::universe);

AA_bounding_box operator+(const AA_bounding_box &aa_bound_box, const vec3 &offset)
{
    return AA_bounding_box(aa_bound_box.x + offset.x(), aa_bound_box.y + offset.y(), aa_bound_box.z + offset.z());
}

AA_bounding_box operator+(const vec3 &offset, const AA_bounding_box &aa_bound_box)
{
    return aa_bound_box + offset;
}

#endif