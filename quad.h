#ifndef QUAD_H
#define QUAD_H

// header file for creating quad objects and figuring out if hit or not

// include
#include "utility.h"
#include "world.h"

// quad
class quad : public hittable
{
public:
    // constructor for quadrilateral with a given point, two vectors, and mats
    quad(const point3 &Q, const vec3 &u, const vec3 &v, shared_ptr<material> mat)
        : Q(Q), u(u), v(v), mat(mat)
    {
        // for debugging
        if (false)
        {
            std::stringstream outFile;
            outFile << "quad: Q(" << Q.x() << "," << Q.y() << "," << Q.z() << ")\n";
            outFile << "vec: u " << u << ", v " << v << "\n";
            outFile << "\n";

            debugger::getInstance().logToFile(outFile.str());
        }

        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);

        set_bounding_box();
    }

    // constructor for quadrilateral with three points and material
    quad(const int differ, const point3 &Q, const point3 &X, const point3 &Y, shared_ptr<material> mat)
        : differ(differ), Q(Q), p_X(X), p_Y(Y), mat(mat)
    {
        u = vec3(p_X - Q);
        v = vec3(p_Y - Q);

        // for debugging
        if (false)
        {
            std::stringstream outFile;
            outFile << "quad: Q(" << Q.x() << "," << Q.y() << "," << Q.z() << ")\n";
            outFile << "quad: X(" << p_X.x() << "," << p_X.y() << "," << p_X.z() << ")\n";
            outFile << "quad: Y(" << p_Y.x() << "," << p_Y.y() << "," << p_Y.z() << ")\n";
            outFile << "vec: u " << u << ", v " << v << "\n";
            outFile << "\n";

            debugger::getInstance().logToFile(outFile.str());
        }

        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);

        set_bounding_box();
    }

    // function to determine if quad has been hit
    bool hit(const ray &r, interval ray_t, place_hit &rec) const override
    {
        auto denom = dot(normal, r.direction());

        // return false if quad has not been hit
        if (std::fabs(denom) < 1e-8)
            return false;

        // return false if outside interval
        auto t = (D - dot(normal, r.origin())) / denom;

        if (!ray_t.contains(t))
        {
            return false;
        }

        // find out if ray hits the quad
        auto intersection = r.at(t);
        vec3 planar_hitpt_vector = intersection - Q;

        auto a = dot(w, cross(planar_hitpt_vector, v));
        auto b = dot(w, cross(u, planar_hitpt_vector));

        // figure out if hit point is in quad
        if (!is_interior(a, b, rec))
        {
            return false;
        }

        // place hit variables
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    // function to determine if hit point is in the quad
    virtual bool is_interior(double a, double b, place_hit &rec) const
    {
        interval unit_interval = interval(0, 1);

        if (!unit_interval.contains(a) || !unit_interval.contains(b))
        {
            return false;
        }

        rec.u = a;
        rec.v = b;

        return true;
    }

    // function to set the bounding box
    virtual void set_bounding_box()
    {
        auto diaganol1 = AA_bounding_box(Q, Q + u + v);
        auto diagonal2 = AA_bounding_box(Q + u, Q + v);

        aa_bound_box = AA_bounding_box(diaganol1, diagonal2);
    }

    // function to return the bounding box
    AA_bounding_box bounding_box() const override
    {
        return aa_bound_box;
    }

private:
    int differ;
    double D;
    point3 Q, p_X, p_Y;
    vec3 u, v, w, normal;
    shared_ptr<material> mat;
    AA_bounding_box aa_bound_box;
};

#endif