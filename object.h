#ifndef OBJECT_H
#define OBJECT_H

#include "utility.h"
#include "OBJ_Loader.h"
#include "material.h"
#include "world.h"
#include "triangle.h"

class object
{
public:
    // Stationary Sphere
    object(const char *filename) : filename(filename)
    {
        loader.LoadFile(filename);
    }

void create_object(world* world)
    {
        // material
        auto yellow = make_shared<diffuse>(color(1, .5, 0));
        auto blue = make_shared<diffuse>(color(.2, .2, 1));

        int mesh_vertices_size = 4;

        // loop over .obj file and add shape to entire object
        for (int i = 0; i < loader.LoadedMeshes.size(); i++)
        {
            objl::Mesh current_mesh = loader.LoadedMeshes[i];

            for (int j = 0; j < current_mesh.Vertices.size(); j += mesh_vertices_size)
            {
                point3 one = point3(current_mesh.Vertices[j].Position.X, current_mesh.Vertices[j].Position.Y, current_mesh.Vertices[j].Position.Z);
                point3 two = point3(current_mesh.Vertices[j + 1].Position.X, current_mesh.Vertices[j + 1].Position.Y, current_mesh.Vertices[j + 1].Position.Z);
                point3 three = point3(current_mesh.Vertices[j + 2].Position.X, current_mesh.Vertices[j + 2].Position.Y, current_mesh.Vertices[j + 2].Position.Z);

                // poly
                if (mesh_vertices_size == 4)
                {
                    point3 four = point3(current_mesh.Vertices[j + 3].Position.X, current_mesh.Vertices[j + 3].Position.Y, current_mesh.Vertices[j + 3].Position.Z);
                    //world.add(make_shared<quad>(1, one, two, three, yellow));

                    world->add(make_shared<triangle>(1, one, two, four, yellow));
                    world->add(make_shared<triangle>(1, three, two, four, yellow));
                }

                // triangle
                else
                {
                    world->add(make_shared<triangle>(1, one, two, three, blue));
                }
            }

            mesh_vertices_size = 3;
        }
    }

private:
    objl::Loader loader;
    std::string filename;
};

#endif