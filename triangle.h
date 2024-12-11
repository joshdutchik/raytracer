#ifndef TRIANGLE_H
#define TRIANGLE_H

// header class for making a triangle hittable object

// include
#include "utility.h"

// triangle
class triangle : public hittable
{
public:
    // constructors
    // create a triangle given a point, two vectors, and material
    triangle(const point3 &Q, const vec3 &u, const vec3 &v, shared_ptr<material> mat)
        : Q(Q), u(u), v(v), mat(mat)
    {
        // for debugging (mainly used with object meshes)
        if (false)
        {
            std::stringstream outFile;
            outFile << "triangle: Q(" << Q.x() << "," << Q.y() << "," << Q.z() << ")\n";
            outFile << "vec: u " << u << ", v " << v << "\n";
            outFile << "\n";

            debugger::getInstance().logToFile(outFile.str());
        }

        // calculations to get opposite vector
        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);

        set_bounding_box();
    }

    // create a triangle given three points and material
    triangle(const int differ, const point3 &Q, const point3 &X, const point3 &Y, shared_ptr<material> mat)
        : Q(Q), p_X(X), p_Y(Y), mat(mat)
    {
        // calculate the vectors between the two points
        v = vec3(p_X - Q);
        u = vec3(p_Y - Q);

        // for debugging (mainly used with object meshes)
        if (false)
        {
            std::stringstream outFile;
            outFile << "triangle: Q(" << Q.x() << "," << Q.y() << "," << Q.z() << ")\n";
            outFile << "triangle: X(" << p_X.x() << "," << p_X.y() << "," << p_X.z() << ")\n";
            outFile << "triangle: Y(" << p_Y.x() << "," << p_Y.y() << "," << p_Y.z() << ")\n";
            outFile << "vec: u " << u << ", v " << v << "\n";
            outFile << "\n";

            debugger::getInstance().logToFile(outFile.str());
        }

        // calculations to get opposite vector
        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);

        set_bounding_box();
    }

    bool hit(const ray &r, interval ray_t, place_hit &rec) const override
    {
        auto denom = dot(normal, r.direction());

        // return if not hit
        if (std::fabs(denom) < 1e-8)
        {
            return false;
        }

        auto t = (D - dot(normal, r.origin())) / denom;

        if (!ray_t.contains(t))
        {
            return false;
        }

        // find out if the ray hits the triangle
        auto intersection = r.at(t);
        vec3 planar = intersection - Q;

        auto a = dot(w, cross(planar, v));
        auto b = dot(w, cross(u, planar));

        if (!is_interior(a, b, rec))
        {
            return false;
        }

        // set the place hit record
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    // see if it hits the triangle, if not return false
    virtual bool is_interior(double a, double b, place_hit &rec) const
    {
        if (a > 0 && b > 0 && (a + b) < 1)
        {
            rec.u = a;
            rec.v = b;
            return true;
        }

        return false;
    }

    // set the bounding box
    virtual void set_bounding_box()
    {
        auto diagonal1 = AA_bounding_box(Q, Q + u + v);
        auto diagonal2 = AA_bounding_box(Q + u, Q + v);

        aa_bound_box = AA_bounding_box(diagonal1, diagonal2);
    }

    // get the bounding box
    AA_bounding_box bounding_box() const override
    {
        return aa_bound_box;
    }

private:
    int differ, debug;
    double D;
    point3 Q, p_X, p_Y;
    vec3 u, v, w, normal;
    shared_ptr<material> mat;
    AA_bounding_box aa_bound_box;
};

#endif