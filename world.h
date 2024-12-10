#ifndef WORLD_H
#define WORLD_H

#include "utility.h"
#include "sphere.h"
#include "triangle.h"
#include "material.h"

class world : public hittable
{
public:
    std::vector<shared_ptr<hittable>> objects;

    world() {}
    world(shared_ptr<hittable> object) { add(object); }
    world(bool floor)
    {
        if (floor)
        {
            add_sphere(point3(0, -100.5, -1), 100, "diffuse", texture_vector(50, 50, 50, 0, 0));
        }
    }

    void add_sphere(point3 center, double radius, std::string mat, texture_vector texture_vector)
    {
        point3 &center_ref = center;

        max_height = center.y() + radius;
        min_height = center.y() - radius;

        shared_ptr<hittable> sphere_object = make_shared<sphere>(center_ref, radius, get_material(mat, texture_vector));

        objects.push_back(sphere_object);
        bbox = aabb(bbox, sphere_object->bounding_box());
    }

    void add_triangle(point3 Q, vec3 u, vec3 v, std::string mat, texture_vector texture_vector)
    {
        point3 &point = Q;
        vec3 &vector_u = u;
        vec3 &vector_v = v;

        max_height = get_max_y(Q.y(), Q.y() + u.y(), Q.y() + v.y());
        min_height = get_min_y(Q.y(), Q.y() + u.y(), Q.y() + v.y());

        shared_ptr<hittable> triangle_object = make_shared<triangle>(point, vector_u, vector_v, get_material(mat, texture_vector));

        objects.push_back(triangle_object);
        bbox = aabb(bbox, triangle_object->bounding_box());
    }

    void add_triangle(int differ, point3 Q, point3 X, point3 Y, std::string mat, texture_vector texture_vector)
    {
        point3 &point_q = Q;
        vec3 &point_x = X;
        vec3 &point_y = Y;

        max_height = get_max_y(Q.y(), X.y(), Q.y() + Y.y());
        min_height = get_min_y(Q.y(), X.y(), Q.y() + Y.y());

        shared_ptr<hittable> triangle_object = make_shared<triangle>(point_q, point_x, point_y, get_material(mat, texture_vector));

        objects.push_back(triangle_object);
        bbox = aabb(bbox, triangle_object->bounding_box());
    }

    void add_quad()
    {
    }

    void add_cube()
    {
    }

    void add(shared_ptr<hittable> object)
    {
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box());
    }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto &object : objects)
        {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec))
            {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

    void clear() { objects.clear(); }

    aabb bounding_box() const override { return bbox; }

private:
    aabb bbox;
    double max_height, min_height;

    shared_ptr<material> get_material(std::string mat, texture_vector texture_vector)
    {
        double red = texture_vector.red() / 100;
        double green = texture_vector.green() / 100;
        double blue = texture_vector.blue() / 100;

        if (mat == "specular")
        {
            return make_shared<specular>(color(red, green, blue), texture_vector.fuzz());
        }

        else if (mat == "diffuse")
        {
            int text_val = texture_vector.texture();

            switch (text_val)
            {
            case 0:
                return make_shared<diffuse>(color(red, green, blue));
            case 1:
                return make_shared<diffuse>(make_shared<sunset>(max_height, min_height));
            case 2:
                return make_shared<diffuse>(make_shared<rainbow>(max_height, min_height));
            case 3:
                return make_shared<diffuse>(make_shared<hashed>(color(red, green, blue)));
            case 4:
                return make_shared<diffuse>(make_shared<perlin_noise>(4, color(red, green, blue)));
            default:
                return make_shared<diffuse>(color(2.55, 1.02, 2.55));
            }
        }

        else if (mat == "dielectric")
        {
            return make_shared<dielectric>(texture_vector.refraction());
        }

        else if (mat == "emissive")
        {
            return make_shared<emissive>(color(red, green, blue));
        }

        else
        {
            return make_shared<diffuse>(color(2.55, 1.02, 2.55));
        }
    }

    double get_max_y(double one, double two, double three, double four = -infinity)
    {
        double max = std::max(one, std::max(two, three));

        if (four > -infinity)
        {
            return std::max(max, four);
        }

        return max;
    }

    double get_min_y(double one, double two, double three, double four = infinity)
    {
        double min = std::min(one, std::min(two, three));

        if (four < infinity)
        {
            return std::min(min, four);
        }

        return min;
    }
};

#endif