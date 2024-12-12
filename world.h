#ifndef WORLD_H
#define WORLD_H

// header file for the "world" that will be rendered

// include statements
#include "utility.h"
#include "sphere.h"
#include "triangle.h"
#include "quad.h"
#include "volume.h"
#include "material.h"

// world class
class world : public hittable
{
public:
    // the "world" in a vector of hittable items
    std::vector<shared_ptr<hittable>> objects;

    // constructors
    world() {}
    world(shared_ptr<hittable> object) { add(object); }
    world(bool floor)
    {
        // for if a default floor in the final render is wanted
        if (floor)
        {
            add_sphere(point3(0, -100.5, -1), 100, "diffuse", texture_vector(50, 50, 50, 0, 0));
        }
    }

    // adds a sphere to the world with a given center, radius, material type, and texture vector
    void add_sphere(point3 center, double radius, std::string mat, texture_vector texture_vector)
    {
        point3 &center_ref = center;

        // height for texture calculations
        max_height = center.y() + radius;
        min_height = center.y() - radius;

        // create sphere
        shared_ptr<hittable> sphere_object = make_shared<sphere>(center_ref, radius, get_material(mat, texture_vector));

        // add to world vector and the bounding box
        objects.push_back(sphere_object);
        aa_bound_box = AA_bounding_box(aa_bound_box, sphere_object->bounding_box());
    }

    // adds a moving sphere to the world with a given center and max movement center, radius, material type, and texture vector
    void add_moving_sphere(point3 center1, point3 center2, double radius, std::string mat, texture_vector tv)
    {
        point3 &center1_ref = center1;
        point3 &center2_ref = center2;

        // for texture calculations
        max_height = std::max((center1.y() + radius), (center2.y() + radius));
        min_height = std::min((center1.y() - radius), (center2.y() - radius));

        // create moving sphere
        shared_ptr<hittable> sphere_object = make_shared<sphere>(center1, center2, radius, get_material(mat, tv));

        // add to world vector
        objects.push_back(sphere_object);
        aa_bound_box = AA_bounding_box(aa_bound_box, sphere_object->bounding_box());
    }

    // adds a triangle to the world with a given point Q, vectors u and v, material type, and texture vector
    void add_triangle(point3 Q, vec3 u, vec3 v, std::string mat, texture_vector texture_vector)
    {
        point3 &point = Q;
        vec3 &vector_u = u;
        vec3 &vector_v = v;

        // calculations for height
        max_height = get_max_y(Q.y(), Q.y() + u.y(), Q.y() + v.y());
        min_height = get_min_y(Q.y(), Q.y() + u.y(), Q.y() + v.y());

        // create triangle
        shared_ptr<hittable> triangle_object = make_shared<triangle>(point, vector_u, vector_v, get_material(mat, texture_vector));

        // add to the world vector
        objects.push_back(triangle_object);
        aa_bound_box = AA_bounding_box(aa_bound_box, triangle_object->bounding_box());
    }

    // adds a triangle to the world with a given point Q, X, and Y, material type, and texture vector
    void add_triangle(int differ, point3 Q, point3 X, point3 Y, std::string mat, texture_vector texture_vector)
    {
        point3 &point_q = Q;
        vec3 &point_x = X;
        vec3 &point_y = Y;

        // calculations for texture
        max_height = get_max_y(Q.y(), X.y(), Y.y());
        min_height = get_min_y(Q.y(), X.y(), Y.y());

        // create triangle
        shared_ptr<hittable> triangle_object = make_shared<triangle>(1, point_q, point_x, point_y, get_material(mat, texture_vector));

        // add to world
        objects.push_back(triangle_object);
        aa_bound_box = AA_bounding_box(aa_bound_box, triangle_object->bounding_box());
    }

    // adds a quad to the world with a given point Q, vectors u and v, material type, and texture vector
    void add_quad(point3 Q, vec3 u, vec3 v, std::string mat, texture_vector texture_vector)
    {
        point3 &point = Q;
        vec3 &vector_u = u;
        vec3 &vector_v = v;

        // for texture calculations
        max_height = get_max_y(Q.y(), Q.y() + u.y(), Q.y() + v.y());
        min_height = get_min_y(Q.y(), Q.y() + u.y(), Q.y() + v.y());

        // create quad object
        shared_ptr<hittable> quad_object = make_shared<quad>(point, vector_u, vector_v, get_material(mat, texture_vector));

        // add to world
        objects.push_back(quad_object);
        aa_bound_box = AA_bounding_box(aa_bound_box, quad_object->bounding_box());
    }

    // adds a quad to the world with a given point Q, X, and Y, material type, and texture vector
    void add_quad(int differ, point3 Q, point3 X, point3 Y, std::string mat, texture_vector texture_vector)
    {
        point3 &point_q = Q;
        vec3 &point_x = X;
        vec3 &point_y = Y;

        // for texture calculations
        max_height = get_max_y(Q.y(), X.y(), Y.y());
        min_height = get_min_y(Q.y(), X.y(), Y.y());

        // create quad
        shared_ptr<hittable> quad_object = make_shared<quad>(point_q, point_x, point_y, get_material(mat, texture_vector));

        // add to world
        objects.push_back(quad_object);
        aa_bound_box = AA_bounding_box(aa_bound_box, quad_object->bounding_box());
    }

    // adds a volume object to the world with a given object, density, material type, and texture vector
    void add_volume(shared_ptr<hittable> fitted_object, double density, std::string mat, texture_vector tv)
    {
        // get volume colors
        double red = tv.red() / 100;
        double green = tv.green() / 100;
        double blue = tv.blue() / 100;

        // create volume object
        shared_ptr<hittable> volume_object = make_shared<volume>(fitted_object, density, color(red, green, blue));

        // add to the world
        objects.push_back(volume_object);
        aa_bound_box = AA_bounding_box(aa_bound_box, volume_object->bounding_box());
    }

    // basic add function for hittable objects
    void add(shared_ptr<hittable> object)
    {
        // add object to world vector
        objects.push_back(object);
        aa_bound_box = AA_bounding_box(aa_bound_box, object->bounding_box());
    }

    // function for detecting hits in the world vector
    bool intersect(const ray &r, interval ray_t, place_hit &rec) const override
    {
        place_hit temp;
        bool is_hit = false;
        auto closest = ray_t.max;

        // loop of the objects that are in the world vector and determine it intersect
        for (const auto &object : objects)
        {
            if (object->intersect(r, interval(ray_t.min, closest), temp))
            {
                is_hit = true;
                closest = temp.t;
                rec = temp;
            }
        }

        return is_hit;
    }

    // function to clear the world
    void clear()
    {
        objects.clear();
    }

    // create the bounding box for acceleration
    AA_bounding_box bounding_box() const override
    {
        return aa_bound_box;
    }

private:
    // heights for texture calculations
    double max_height, min_height;
    // the axis-aligned bounding box
    AA_bounding_box aa_bound_box;

    // function to sort material with correct material and apply the texture defined in the vector
    shared_ptr<material> get_material(std::string mat, texture_vector texture_vector)
    {
        // divide rgb values
        double red = texture_vector.red() / 100;
        double green = texture_vector.green() / 100;
        double blue = texture_vector.blue() / 100;

        // specular
        if (mat == "specular")
        {
            return make_shared<specular>(color(red, green, blue), texture_vector.fuzz());
        }

        // diffuse (uses texture, need to find texture values based on the vector)
        else if (mat == "diffuse")
        {
            int text_val = texture_vector.texture();

            // define which texture is selected
            switch (text_val)
            {
            case 0:
                // solid color
                return make_shared<diffuse>(color(red, green, blue));
            case 1:
                // sunset
                return make_shared<diffuse>(make_shared<sunset>(max_height, min_height));
            case 2:
                // rainbow
                return make_shared<diffuse>(make_shared<rainbow>(max_height, min_height));
            case 3:
                // hashed
                return make_shared<diffuse>(make_shared<hashed>(color(red, green, blue)));
            case 4:
                // perlin
                return make_shared<diffuse>(make_shared<perlin_noise>(4, color(red, green, blue)));
            default:
                // debug color
                return make_shared<diffuse>(color(2.55, 1.02, 2.55));
            }
        }

        // dielectric
        else if (mat == "dielectric")
        {
            return make_shared<dielectric>(texture_vector.refraction());
        }

        // lights
        else if (mat == "emissive")
        {
            return make_shared<emissive>(color(red, green, blue));
        }

        // volume rendering
        else if (mat == "volume")
        {
            return make_shared<volume_mat>(color(red, green, blue));
        }

        // color for debugged material
        else
        {
            return make_shared<diffuse>(color(2.55, 1.02, 2.55));
        }
    }

    // get the max height of the shape for calculations
    double get_max_y(double one, double two, double three, double four = -infinity)
    {
        double max = std::max(one, std::max(two, three));

        if (four > -infinity)
        {
            return std::max(max, four);
        }

        return max;
    }

    // get the min height of the shape for texture calculations
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