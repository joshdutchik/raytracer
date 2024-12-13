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
    void create_object(world *world, point3 center, double scale)
    {
        // default material
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
                one += center;

                // loop over face size and get points for each face by splitting into triangles
                for (int k = 1; k < (face_size-1); k++)
                {
                    int two_index = j + k;
                    int three_index = j + (k + 1);

                    debugger::getInstance().logToFile(std::to_string(j));
                    debugger::getInstance().logToFile(std::to_string(two_index));
                    debugger::getInstance().logToFile(std::to_string(three_index));
                    debugger::getInstance().logToFile("---------------");

                    point3 two = point3(current_mesh.Vertices[two_index].Position.X, current_mesh.Vertices[two_index].Position.Y, current_mesh.Vertices[two_index].Position.Z);
                    two += center;

                    point3 three = point3(current_mesh.Vertices[three_index].Position.X, current_mesh.Vertices[three_index].Position.Y, current_mesh.Vertices[three_index].Position.Z);
                    three += center;

                    world->add(make_shared<triangle>(1, one*scale, two*scale, three*scale, blue));
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