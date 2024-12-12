// main .cpp file to run custom interface to test features of the raytracer

// include
#include "utility.h"
#include "menu.h"

// main function
int main()
{
    // Program shows:
    // A camera with configurable position, orientation, and field of view
    // Anti-aliasing
    // Ray/sphere intersections
    // Ray/triangle intersections
    // The ability to load textures (file format(s) of your choice; may use third-party libraries)
    // Textured spheres and triangles
    // The ability to load and render triangle meshes (file format(s) of your choice; may use third-party libraries for loading)
    // A spatial subdivision acceleration structure of your choice
    // Specular, diffuse, and dielectric materials (per first volume of Ray Tracing in One Weekend series)
    // Emissive materials (lights)
    // Volume rendering
    // quads
    // motion blur
    // perlin noise

    // activate menu unless quit
    int choice = 0;

    while (choice != 4)
    {
        displayStarterMenu();
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            displayBFMenu();
            break;
        case 2:
            displayEFMenu();
            break;
        case 3:
            final_render();
            break;
        case 4:
            std::cout << "EXITING...";
            break;
        default:
            std::cout << "Invalid choice, please select a valid option: ";
        }
    }

    return 0;
}