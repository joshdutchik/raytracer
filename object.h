#ifndef OBJECT_H
#define OBJECT_H

// header file to create a triangle mesh of either 3 or 4 face vertices using the OBJ_Loader.h utility and
// given .obj file

// include
#include "utility.h"
#include "OBJ_Loader.h"
#include "material.h"
#include "world.h"
#include "triangle.h"

// object
class object
{
public:
    // constructor to create a triangle mesh object with given name
    object(const char *filename) : filename(filename)
    {
        // open file
        std::ifstream object_file(filename);

        if (!object_file.is_open())
        {
            debugger::getInstance().logToFile("Cannot find object file");
            return;
        }

        // find faces of the object and each mesh
        std::string line;

        while (std::getline(object_file, line))
        {
            if (!line.empty() && line[0] == 'f')
            {
                std::istringstream iss(line);
                std::string part;

                int partCount = -1;

                while (iss >> part)
                {
                    partCount++;
                }

                if (partCount > 0)
                {
                    face_vertice_count.push_back(partCount);
                }
            }
        }

        object_file.close();
        loader.LoadFile(filename);
    }

    // create object inside the world on the given center point
    void create_object(world *world, point3 center)
    {
        // material
        auto yellow = make_shared<diffuse>(color(1, .5, 0));
        auto blue = make_shared<diffuse>(color(.2, .2, 1));

        int face_index = 0;

        // loop over .obj file and add shape to entire object
        for (int i = 0; i < loader.LoadedMeshes.size(); i++)
        {
            objl::Mesh current_mesh = loader.LoadedMeshes[i];

            // go over each mesh vertices
            for (int j = 0; j < current_mesh.Vertices.size();)
            {
                int face_size = face_vertice_count[face_index];

                point3 one = point3(current_mesh.Vertices[j].Position.X, current_mesh.Vertices[j].Position.Y, current_mesh.Vertices[j].Position.Z);
                point3 two = point3(current_mesh.Vertices[j + 1].Position.X, current_mesh.Vertices[j + 1].Position.Y, current_mesh.Vertices[j + 1].Position.Z);
                point3 three = point3(current_mesh.Vertices[j + 2].Position.X, current_mesh.Vertices[j + 2].Position.Y, current_mesh.Vertices[j + 2].Position.Z);

                one = one + center;
                two = two + center;
                three = three + center;

                // poly
                if (face_size == 4)
                {
                    point3 four = point3(current_mesh.Vertices[j + 3].Position.X, current_mesh.Vertices[j + 3].Position.Y, current_mesh.Vertices[j + 3].Position.Z);
                    four = four + center;

                    world->add(make_shared<triangle>(1, one, two, four, yellow));
                    world->add(make_shared<triangle>(1, three, two, four, yellow));
                }

                // triangle
                else
                {
                    world->add(make_shared<triangle>(1, one, two, three, blue));
                }

                j += face_size;
                face_index++;
            }
        }
    }

private:
    objl::Loader loader;
    std::string filename;
    std::vector<int> face_vertice_count;
};

#endif