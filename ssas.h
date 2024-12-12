#ifndef SSAS_H
#define SSAS_H

// header for the spatial subdivision acceleration structure

// include
#include "utility.h"
#include "AA_bounding_box.h"
#include "world.h"

class spatial_sub_acc_struct : public hittable
{
public:
    // constructor for with given world
    spatial_sub_acc_struct(world list) : spatial_sub_acc_struct(list.objects, 0, list.objects.size()) {}

    // constructor for vector of hittable objects
    spatial_sub_acc_struct(std::vector<shared_ptr<hittable>> &objects, size_t start, size_t end)
    {
        aa_bound_box = AA_bounding_box::empty;

        for (size_t object_index = start; object_index < end; object_index++)
        {
            aa_bound_box = AA_bounding_box(aa_bound_box, objects[object_index]->bounding_box());
        }

        int axis = aa_bound_box.longest_axis();
        auto comparator = box_z_compare;

        if (axis == 0)
        {
            comparator = box_x_compare;
        }

        else if (axis == 1)
        {
            comparator = box_y_compare;
        }

        size_t object_span = end - start;

        if (object_span == 1)
        {
            left = right = objects[start];
        }

        else if (object_span == 2)
        {
            left = objects[start];
            right = objects[start + 1];
        }

        else
        {
            std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

            auto mid = start + object_span / 2;
            left = make_shared<spatial_sub_acc_struct>(objects, start, mid);
            right = make_shared<spatial_sub_acc_struct>(objects, mid, end);
        }
    }

    bool intersect(const ray &r, interval ray_t, place_hit &rec) const override
    {
        if (!aa_bound_box.intersect(r, ray_t))
        {
            return false;
        }

        bool hit_left = left->intersect(r, ray_t, rec);
        bool hit_right = right->intersect(r, interval(ray_t.min, hit_left ? rec.t : ray_t.max), rec);

        return hit_left || hit_right;
    }

    AA_bounding_box bounding_box() const override { return aa_bound_box; }

private:
    shared_ptr<hittable> left;
    shared_ptr<hittable> right;

    AA_bounding_box aa_bound_box;

    static bool box_compare(
        const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index)
    {
        auto a_axis_interval = a->bounding_box().axis_interval(axis_index);
        auto b_axis_interval = b->bounding_box().axis_interval(axis_index);

        return a_axis_interval.min < b_axis_interval.min;
    }

    // helper functions
    static bool box_x_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
    {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
    {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare(const shared_ptr<hittable> a, const shared_ptr<hittable> b)
    {
        return box_compare(a, b, 2);
    }
};

#endif