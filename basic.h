#ifndef BASIC_H
#define BASIC_H

#include "rtweekend.h"

void configurable_camera()
{
    // create world
    hittable_list world;

    // create material
    auto ground_material = make_shared<lambertian>(color(.5, .5, .5));
    auto ball_material = make_shared<lambertian>(color(.4, .2, .1));

    // add to the world
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
    world.add(make_shared<sphere>(point3(-1, 1, 0), 1, ball_material));
    world.add(make_shared<sphere>(point3(1, 1, 0), 1, ball_material));

    // camera settings
    camera cam(400, 225, color(0.70, 0.80, 1.00));

    // close up, in the middle, looking up
    cam.configure(10, point3(0, 0.5, 13), point3(0, 1, 0), vec3(0, 1, 0));
    cam.render(world, "configurable-camera-10.ppm");

    // normal zoom, on the side, looking straight
    cam.configure(25, point3(-7, 1, 0), point3(0, 1, 2), vec3(0, 1, 0));
    cam.render(world, "configurable-camera-25.ppm");

    // far away, to the side, looking down
    cam.configure(90, point3(2, 3, -2), point3(0, 1, 0), vec3(0, 1, 0));
    cam.render(world, "configurable-camera-90.ppm");
}

void anti_aliasing()
{
    // create world
    hittable_list world;

    // create material
    auto ground_material = make_shared<lambertian>(color(.5, .5, .5));
    auto ball_material = make_shared<lambertian>(color(.4, .2, .1));

    // add to the world
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
    world.add(make_shared<sphere>(point3(0, 1, 0), 1, ball_material));

    // camera settings
    camera cam(400, 225, color(0.70, 0.80, 1.00));
    cam.configure(20, point3(0, 2, 6), point3(0, 1, 0), vec3(0, 1, 0));

    // without anti-aliasing
    cam.render(world, "anti-aliasing-without.ppm", false);

    // with anti-aliasing
    cam.render(world, "anti-aliasing-with.ppm");
}

void sphere_intersections()
{
    // create world
    hittable_list world;

    // camera settings
    camera cam(400, 225, color(0.70, 0.80, 1.00));
    cam.configure(90, point3(0, 2, 6), point3(0, 2, 0), vec3(0, 1, 0));

    // create material
    auto ground_material = make_shared<lambertian>(color(.5, .5, .5));
    auto ball_material = make_shared<lambertian>(color(.4, .2, .1));

    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));
    world.add(make_shared<sphere>(point3(6, 3, 0), 3, ball_material));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, ball_material));
    world.add(make_shared<sphere>(point3(-5, 1, 0), 1, ball_material));

    cam.render(world, "spheres.ppm");
}

void triangle_intersections()
{
    // create world
    hittable_list world;

    // camera settings
    camera cam(400, 225, color(0.70, 0.80, 1.00));
    cam.configure(80, point3(0, 0, 9), point3(0, 0, 0), vec3(0, 1, 0));

    // triangles
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
     world.add(make_shared<quad>(point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));

    cam.render(world, "triangles.ppm");
}

void load_textures();
void textured_spheres_triangles();
void triangle_meshes();
void acceleration_structure();
void materials();
void lights();

#endif