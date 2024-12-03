#ifndef BASIC_H
#define BASIC_H

#include "rtweekend.h"

void configurable_camera()
{
    // create world
    hittable_list world;

    // ground
    auto ground_material = make_shared<lambertian>(color(.5, .5, .5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    // spheres
    auto ball_material = make_shared<lambertian>(color(.4, .2, .1));
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

    // ground
    auto ground_material = make_shared<lambertian>(color(.5, .5, .5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    // sphere
    auto ball_material = make_shared<lambertian>(color(.4, .2, .1));
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

    // ground material
    auto ground_material = make_shared<lambertian>(color(.5, .5, .5));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    // spheres
    auto ball_material = make_shared<lambertian>(color(.4, .2, .1));
    world.add(make_shared<sphere>(point3(6, 3, 0), 3, ball_material));
    world.add(make_shared<sphere>(point3(0, 2, 0), 2, ball_material));
    world.add(make_shared<sphere>(point3(-5, 1, 0), 1, ball_material));

    // render
    cam.render(world, "spheres.ppm");
}

void triangle_intersections()
{
    // create world
    hittable_list world;

    // camera settings
    camera cam(400, 225, color(0.70, 0.80, 1.00));
    cam.configure(80, point3(0, 0, 9), point3(0, 0, 0), vec3(0, 1, 0));

    // bottom left triangle
    auto red = make_shared<lambertian>(color(1, .2, .2));
    world.add(make_shared<triangle>(point3(-2, 3, 0), vec3(-7, 4, 0), vec3(-7, -1, 0), red));

    // bottom right triangle
    auto blue = make_shared<lambertian>(color(.2, .2, 1));
    world.add(make_shared<triangle>(point3(5, 2, 0), vec3(7, -2, 0), vec3(7, -4, 0), blue));

    // top
    auto green = make_shared<lambertian>(color(.2, 1, .2));
    world.add(make_shared<triangle>(point3(0, 0, 0), vec3(4, 0, 0), vec3(0, 4, 0), green));

    // center
    auto yellow = make_shared<lambertian>(color(1, .5, 0));
    world.add(make_shared<triangle>(point3(0, -6, 0), vec3(-4, 4, 0), vec3(4, 4, 0), yellow));

    // render
    cam.render(world, "triangles.ppm");
}

void load_textures() {
    // create world
    hittable_list world;

    // camera settings
    camera cam(400, 225, color(0.70, 0.80, 1.00));
    cam.configure(90, point3(16, 8, 0), point3(0, 5, 0), vec3(0, 1, 0));

    camera cam2(400, 225, color(0.70, 0.80, 1.00));
    cam2.configure(90, point3(0, 8, 16), point3(0, 5, 0), vec3(0, 1, 0));

    camera cam3(400, 225, color(0.70, 0.80, 1.00));
    cam3.configure(90, point3(-16, 8, 0), point3(0, 5, 0), vec3(0, 1, 0));

    camera cam4(400, 225, color(0.70, 0.80, 1.00));
    cam4.configure(90, point3(0, 8, -16), point3(0, 5, 0), vec3(0, 1, 0));

    // ground material
    auto ground_material = make_shared<lambertian>(color(.5, .5, .5));
    auto ball_surface = make_shared<lambertian>(make_shared<hashed>());
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ball_surface));

    // spheres
    world.add(make_shared<sphere>(point3(0, 8, 0), 8, ground_material));

    // render
    cam.render(world, "loaded-texture.ppm");
    //cam2.render(world, "loaded-texture2.ppm");
    //cam3.render(world, "loaded-texture3.ppm");
    //cam4.render(world, "loaded-texture4.ppm");
}

void textured_spheres_triangles() {

}

void triangle_meshes();
void materials();
void lights();

#endif