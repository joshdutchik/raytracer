#ifndef MENU_H
#define MENU_H

// header file for menu used to find the wanted input

// include
#include "utility.h"
#include "basic.h"

// function for display of starter menu
inline void displayStarterMenu()
{
    std::cout << "===== Josh Dutchik's COMS 336 Raytracer =====" << std::endl;
    std::cout << "1: Basic Functionalites" << std::endl;
    std::cout << "2: Extra Functionalites" << std::endl;
    std::cout << "3: Final Render" << std::endl;
    std::cout << "4: Quit" << std::endl;
    std::cout << "Select an option (1-4): ";
}

// basic menu options
inline void BFOptions()
{
    int choice = 0;

    while (choice != 8)
    {
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            std::cout << "Configurable Camera";
            std::cout << "\n";
            configurable_camera();
            break;
        case 2:
            std::cout << "Anti-Aliasing";
            std::cout << "\n";
            anti_aliasing();
            break;
        case 3:
            std::cout << "Ray/sphere intersections";
            std::cout << "\n";
            sphere_intersections();
            break;
        case 4:
            std::cout << "Ray/triangle intersections";
            std::cout << "\n";
            triangle_intersections();
            break;
        case 5:
            std::cout << "Textured spheres and triangles";
            std::cout << "\n";
            load_textures();
            break;
        case 6:
            std::cout << "Load and Render Trianlge Meshes";
            std::cout << "\n";
            triangle_meshes();
            break;
        case 7:
            std::cout << "Specular, diffuse, and dielectric materials";
            std::cout << "\n";
            materials();
            break;
        case 8:
            std::cout << "Emissive materials";
            std::cout << "\n";
            lights();
            break;
        case 9:
            break;
        default:
            std::cout << "Invalid choice, please select a valid option: ";
            continue;
        }

        return;
    }
}

// basic functionalities display menu
inline void displayBFMenu()
{
    std::cout << "\n";
    std::cout << "===== Basic Functionalites =====" << std::endl;
    std::cout << "1: Configurable camera" << std::endl;
    std::cout << "2: Anti-aliasing" << std::endl;
    std::cout << "3: Ray/sphere intersections" << std::endl;
    std::cout << "4: Ray/triangle intersections" << std::endl;
    std::cout << "5: Textured spheres and triangles" << std::endl;
    std::cout << "6: Load and render triangle meshes" << std::endl;
    std::cout << "7: Specular, diffuse, and dielectric materials" << std::endl;
    std::cout << "8: Emissive materials" << std::endl;
    std::cout << "9: Back" << std::endl;
    std::cout << "Select an option (1-9): ";

    BFOptions();
}

// extra functionalities options
inline void EFOptions()
{
    int choice = 0;

    while (choice != 5)
    {
        std::cin >> choice;

        switch (choice)
        {
        case 1:
            std::cout << "Quads";
            std::cout << "\n";
            quads();
            break;
        case 2:
            std::cout << "Motion blur";
            std::cout << "\n";
            motion_blur();
            break;
        case 3:
            std::cout << "Perlin noise";
            std::cout << "\n";
            perlin_noise_example();
            break;
        case 4:
            std::cout << "Volume rendering";
            std::cout << "\n";
            volume_rendering();
            break;
        case 5:
            break;
        default:
            std::cout << "Invalid choice, please select a valid option." << std::endl;
            continue;
        }

        return;
    }
}

// extra functionalites menu
inline void displayEFMenu()
{
    std::cout << "===== Extra Functionalites =====" << std::endl;
    std::cout << "1. Quads (10)" << std::endl;
    std::cout << "2. Motion blur (10)" << std::endl;
    std::cout << "3. Perlin noise (10)" << std::endl;
    std::cout << "4. Volume rendering (10)" << std::endl;
    std::cout << "5: Back" << std::endl;
    std::cout << "Select an option (1-5): ";

    EFOptions();
}

#endif