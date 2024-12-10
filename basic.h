#ifndef BASIC_H
#define BASIC_H

#include "utility.h"
#include "world.h"
#include "camera.h"
#include "material.h"
#include "texture.h"
#include "sphere.h"
#include "quad.h"
#include "triangle.h"
#include "object.h"

void configurable_camera()
{
    // create scene
    world scene(true);

    // texture vector
    texture_vector tv = texture_vector(40, 20, 10);

    // spheres
    scene.add_sphere(point3(-1, 0.5, 0), 1, "diffuse", tv);
    scene.add_sphere(point3(1, 0.5, 0), 1, "diffuse", tv);

    // camera settings
    camera cam;

    // close up, in the middle, looking up
    cam.configure(10, point3(0, 0.5, 13), point3(0, 1, 0), vec3(0, 1, 0));
    cam.render(scene, "configurable-camera-10.ppm");

    // normal zoom, on the side, looking straight
    cam.configure(25, point3(-7, 1, 0), point3(0, 1, 2), vec3(0, 1, 0));
    cam.render(scene, "configurable-camera-25.ppm");

    // far away, to the side, looking down
    cam.configure(90, point3(2, 3, -2), point3(0, 1, 0), vec3(0, 1, 0));
    cam.render(scene, "configurable-camera-90.ppm");
}

void anti_aliasing()
{
    // create scene
    world scene(true);

    // camera settings
    camera cam;
    cam.configure(20, point3(0, 2, 6), point3(0, 1, 0), vec3(0, 1, 0));

    // sphere
    scene.add_sphere(point3(0, 1, 0), 1, "diffuse", texture_vector(40, 20, 10));

    // without anti-aliasing
    cam.render(scene, "anti-aliasing-without.ppm", false);

    // with anti-aliasing
    cam.render(scene, "anti-aliasing-with.ppm");
}

void sphere_intersections()
{
    // create scene
    world scene(true);

    // camera settings
    camera cam;
    cam.configure(90, point3(0, 2, 6), point3(0, 2, 0), vec3(0, 1, 0));

    // texture vector
    texture_vector tv = texture_vector(40, 20, 10);

    // spheres
    scene.add_sphere(point3(6, 2.5, 0), 3, "diffuse", tv);
    scene.add_sphere(point3(0, 1.5, 0), 2, "diffuse", tv);
    scene.add_sphere(point3(-5, 0.5, 0), 1, "diffuse", tv);

    // render
    cam.render(scene, "spheres.ppm");
}

void triangle_intersections()
{
    // create scene
    world scene;

    // camera settings
    camera cam;
    cam.configure(80, point3(0, 0, 9), point3(0, 0, 0), vec3(0, 1, 0));

    // bottom left red triangle
    scene.add_triangle(point3(-2, 3, 0), vec3(-7, 4, 0), vec3(-7, -1, 0), "diffuse", texture_vector(100, 20, 20));

    // bottom right triangle
    scene.add_triangle(point3(5, 2, 0), vec3(7, -2, 0), vec3(7, -4, 0), "diffuse", texture_vector(20, 20, 100));

    // top green
    scene.add_triangle(point3(0, 0, 0), vec3(4, 0, 0), vec3(0, 4, 0), "diffuse", texture_vector(20, 100, 20));

    // center yellow
    scene.add_triangle(point3(0, -6, 0), vec3(-4, 4, 0), vec3(4, 4, 0), "diffuse", texture_vector(100, 50, 0));

    // render
    cam.render(scene, "triangles.ppm");
}

void load_textures()
{
    // create scene
    world scene(true);

    // camera settings
    camera cam;
    cam.configure(70, point3(0, 4, 8), point3(0, 3, 0), vec3(0, 1, 0));

    texture_vector tv = texture_vector(0,0,204);

    // spheres
    scene.add_sphere(point3(6, 2.5, 0), 3, "diffuse", texture_vector(1));
    scene.add_sphere(point3(0, 1.5, 0), 2, "diffuse", texture_vector(2));
    scene.add_sphere(point3(-5, 0.5, 0), 1, "diffuse", texture_vector(204,204,204,0,0,3));
    scene.add_sphere(point3(-9, 0.5, 0), 1, "diffuse", texture_vector(0,0,204,0,0,4));

    // render
    cam.render(scene, "loaded-texture.ppm");
}

void triangle_meshes()
{
    // create scene
    world scene;

    // camera settings
    camera cam(400, 225, color(0.70, 0.80, 1.00));
    cam.configure(110, point3(16, 8, 0), point3(12, 10, 2), vec3(0, 1, 0));

    // load objects
    object loaded_mesh = object("Objects/tree.obj");
    loaded_mesh.create_object(&scene);

    // render
    cam.render(scene, "triangle-mesh.ppm");
}

void materials()
{
    // create scene
    world scene(true);

    // camera settings
    camera cam(400, 225);
    cam.configure(70, point3(0, 0, 1), point3(0, 0, -1.2), vec3(0, 1, 0));

    // specular
    scene.add_sphere(point3(0, 0, -1.2), 0.5, "specular", texture_vector(80, 80, 0));

    // diffuse
    scene.add_sphere(point3(-1, 0, -1), 0.5, "diffuse", texture_vector(10, 20, 50));

    // dielectric
    scene.add_sphere(point3(1, 0, -1), 0.5, "dielectric", texture_vector(0, 0, 0, 0, 1.5));

    // render
    cam.render(scene, "materials.ppm");
}

void lights()
{
    // create scene
    world scene(true);

    // camera settings
    camera cam(400, 225, color(0,0,0));
    cam.configure(20, point3(26, 3, 6), point3(0, 2, 0), vec3(0, 1, 0));

    // sphere
    scene.add_sphere(point3(0, 1.5, 0), 2, "diffuse", texture_vector(10, 20, 50));

    // light
    scene.add_triangle(point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), "emissive", texture_vector(255, 255, 555));

    // render
    cam.render(scene, "lights.ppm");
}

void quads() {

}

void motion_blur() {

}

void perlin_noise() {

}

void volume_rendering() {

}

#endif