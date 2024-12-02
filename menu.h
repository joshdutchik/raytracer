#ifndef MENU_H
#define MENU_H

#include <iostream>
#include "basic.h"

inline void displayStarterMenu()
{
    std::cout << "===== Josh Dutchik's COMS 336 Raytracer =====" << std::endl;
    std::cout << "1: Basic Functionalites" << std::endl;
    std::cout << "2: Extra Functionalites" << std::endl;
    std::cout << "3: Functionalites Combined" << std::endl;
    std::cout << "4: Random Things I created" << std::endl;
    std::cout << "5: Quit" << std::endl;
    std::cout << "Select an option (1-5): ";
}

inline void BFOptions()
{
    int choice = 0;

    while (choice != 11)
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
            // need
            break;
        case 6:
            // need
            break;
        case 7:
            // need
            break;
        case 8:
            // need
            break;
        case 9:
            // need
            break;
        case 10:
            // need
            break;
        case 11:
            return;
        default:
            std::cout << "Invalid choice, please select a valid option: ";
            continue;
        }

        return;
    }
}

inline void displayBFMenu()
{
    std::cout << "\n";
    std::cout << "===== Basic Functionalites =====" << std::endl;
    std::cout << "1: Configurable camera" << std::endl;
    std::cout << "2: Anti-aliasing" << std::endl;
    std::cout << "3: Ray/sphere intersections" << std::endl;
    std::cout << "4: Ray/triangle intersections" << std::endl;
    std::cout << "5: Load textures" << std::endl;
    std::cout << "6: Textured spheres and triangles" << std::endl;
    std::cout << "7: Load and render triangle meshes" << std::endl;
    std::cout << "8: Spatial subdivision acceleration structure" << std::endl;
    std::cout << "9: Specular, diffuse, and dielectric materials" << std::endl;
    std::cout << "10: Emissive materials" << std::endl;
    std::cout << "11: Back" << std::endl;
    std::cout << "Select an option (1-11): ";

    BFOptions();
}

inline void displayEFMenu()
{
    std::cout << "===== Extra Functionalites =====" << std::endl;
    std::cout << "1. Quads (10)" << std::endl;
    std::cout << "2. Motion blur (10)" << std::endl;
    std::cout << "3. Perlin noise (10)" << std::endl;
    std::cout << "4. Volume rendering (10)" << std::endl;
    std::cout << "5: Back" << std::endl;
    std::cout << "Select an option (1-5): ";
}

inline void EFOptions(int choice)
{
    switch (choice)
    {
    case 1:
        // need
        break;
    case 2:
        // need
        break;
    case 3:
        // need
        break;
    case 4:
        // need
        break;
    case 5:
        displayStarterMenu();
        break;
    default:
        std::cout << "Invalid choice, please select a valid option." << std::endl;
    }
}

inline void displayFCMenu()
{
    std::cout << "===== Functionalites Combined =====" << std::endl;
    std::cout << "4. Back" << std::endl;
    std::cout << "Select an option (1-4): ";
}

inline void FCOptions(int choice)
{
    switch (choice)
    {
    case 1:
        displayBFMenu();
        break;
    case 2:
        displayEFMenu();
        break;
    case 3:
        displayFCMenu();
        break;
    case 4:
        displayStarterMenu();
        break;
    default:
        std::cout << "Invalid choice, please select a valid option." << std::endl;
    }
}

inline void displayRandomMenu()
{
    std::cout << "===== Random Creations =====" << std::endl;
    std::cout << "1. Random 1" << std::endl;
    std::cout << "2. Random 2" << std::endl;
    std::cout << "3. Random 3" << std::endl;
    std::cout << "4. Back" << std::endl;
    std::cout << "Select an option (1-4): ";
}

inline void randomOptions(int choice)
{
    switch (choice)
    {
    case 1:
        // need
        break;
    case 2:
        // need
        break;
    case 3:
        // need
        break;
    case 4:
        // need
        break;
    default:
        std::cout << "Invalid choice, please select a valid option." << std::endl;
    }
}

#endif // MENU_H