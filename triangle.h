#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "utility.h"

class triangle : public hittable
{
public:
    triangle(const point3 &Q, const vec3 &u, const vec3 &v, shared_ptr<material> mat)
        : Q(Q), u(u), v(v), mat(mat)
    {
        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);

        set_bounding_box();
    }

    triangle(const int differ, const point3 &Q, const point3 &X, const point3 &Y, shared_ptr<material> mat) 
        : Q(Q), p_X(X), p_Y(Y), mat(mat)
    {
        v = vec3(p_X - Q);
        u = vec3(p_Y - Q);

        /*
        std::stringstream outFile;
        outFile << "triangle: W(" << p_W.x() << "," << p_W.y() << "," << p_W.z() << ")\n";
        outFile << "triangle: X(" << p_X.x() << "," << p_X.y() << "," << p_X.z() << ")\n";
        outFile << "triangle: Y(" << p_Y.x() << "," << p_Y.y() << "," << p_Y.z() << ")\n";
        outFile << "vec: u " << u << ", v " << v << "\n";
        outFile << "\n";

        debugger::getInstance().logToFile(outFile.str());
        */

        auto n = cross(u, v);
        normal = unit_vector(n);
        D = dot(normal, Q);
        w = n / dot(n, n);

        set_bounding_box();
    }

    virtual void set_bounding_box()
    {
        // Compute the bounding box of all four vertices.
        auto bbox_diagonal1 = aabb(Q, Q + u + v);
        auto bbox_diagonal2 = aabb(Q + u, Q + v);
        bbox = aabb(bbox_diagonal1, bbox_diagonal2);
    }

    aabb bounding_box() const override { return bbox; }

    bool hit(const ray &r, interval ray_t, hit_record &rec) const override
    {
        auto denom = dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (std::fabs(denom) < 1e-8)
        {
            return false;
        }

        // Return false if the hit point parameter t is outside the ray interval.
        auto t = (D - dot(normal, r.origin())) / denom;
        
        if (!ray_t.contains(t))
        {
            return false;
        }

        // Determine if the hit point lies within the planar shape using its plane coordinates.
        auto intersection = r.at(t);

        vec3 planar_hitpt_vector = intersection - Q;
        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        auto beta = dot(w, cross(u, planar_hitpt_vector));

        if (!is_interior(alpha, beta, rec))
        {
            return false;
        }

        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(double a, double b, hit_record &rec) const
    {
        if (a > 0 && b > 0 && (a + b) < 1)
        {
            rec.u = a;
            rec.v = b;
            return true;
        }

        return false;
    }

private:
    int differ;
    point3 Q;
    point3 p_X,p_Y;
    vec3 u, v;
    vec3 w;
    shared_ptr<material> mat;
    aabb bbox;
    vec3 normal;
    double D;
};

#endif