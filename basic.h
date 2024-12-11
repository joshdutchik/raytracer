#ifndef BASIC_H
#define BASIC_H

// header file to run basic functionalities of the raytracer

// include
#include "utility.h"
#include "world.h"
#include "camera.h"
#include "material.h"
#include "texture.h"
#include "sphere.h"
#include "quad.h"
#include "triangle.h"
#include "object.h"

// configurable camera
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

// anti-aliasing
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

// spheres
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

// triangles
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

// loaded textures
void load_textures()
{
    // create scene
    world scene(true);

    // camera settings
    camera cam;
    cam.configure(70, point3(0, 4, 8), point3(0, 3, 0), vec3(0, 1, 0));

    texture_vector tv = texture_vector(0, 0, 204);

    // spheres
    scene.add_sphere(point3(6, 2.5, 0), 3, "diffuse", texture_vector(1));
    scene.add_sphere(point3(0, 1.5, 0), 2, "diffuse", texture_vector(2));
    scene.add_sphere(point3(-5, 0.5, 0), 1, "diffuse", texture_vector(204, 204, 204, 0, 0, 3));
    scene.add_sphere(point3(-9, 0.5, 0), 1, "diffuse", texture_vector(0, 0, 204, 0, 0, 4));

    // render
    cam.render(scene, "loaded-texture.ppm");
}

// load and render triangle meshes
void triangle_meshes()
{
    // create scene
    world scene;

    // camera settings
    camera cam(400, 225, color(0.70, 0.80, 1.00));
    cam.configure(110, point3(16, 8, -5), point3(8, 12, 2), vec3(0, 1, 0));

    // load objects
    object loaded_mesh = object("Objects/tree.obj");
    loaded_mesh.create_object(&scene, point3(0, 0, 0));

    // render
    cam.render(scene, "triangle-mesh.ppm");
}

// specular, diffuse, dielectric
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

// emissive materials
void lights()
{
    // create scene
    world scene(true);

    // camera settings
    camera cam(400, 225, color(0, 0, 0));
    cam.configure(20, point3(26, 3, 6), point3(0, 2, 0), vec3(0, 1, 0));

    // sphere
    scene.add_sphere(point3(0, 1.5, 0), 2, "diffuse", texture_vector(10, 20, 50));

    // light
    scene.add_triangle(point3(3, 1, -2), vec3(2, 0, 0), vec3(0, 2, 0), "emissive", texture_vector(255, 255, 555));

    // render
    cam.render(scene, "lights.ppm");
}

// quads
void quads()
{
    // create scene
    world scene;

    // camera settings
    camera cam;
    cam.configure(80, point3(0, 0, 9), point3(0, 0, 0), vec3(0, 1, 0));

    // texture vector
    texture_vector tv = texture_vector(100, 20, 20);

    // add quads
    scene.add_quad(point3(-2, -2, 0), vec3(4, 0, 0), vec3(0, 4, 0), "diffuse", tv);
    scene.add_quad(point3(3, -2, 1), vec3(0, 0, 4), vec3(0, 4, 0), "diffuse", tv);
    scene.add_quad(point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), "diffuse", tv);

    // render
    cam.render(scene, "quads.ppm");
}

// motion blur
void motion_blur()
{
    // create scene
    world scene(true);

    // camera
    camera cam;
    cam.configure(20, point3(26, 3, 6), point3(0, 2, 0), vec3(0, 1, 0));

    // texture vector
    texture_vector tv = texture_vector(100, 20, 20);

    // add sphere
    scene.add_moving_sphere(point3(0, 1.5, 0), point3(0, 2, 0), 2, "diffuse", tv);

    // render
    cam.render(scene, "motion-blur.ppm");
}

// perlin noise
void perlin_noise_example()
{
    // create scene
    world scene;

    // camera settings
    camera cam;
    cam.configure(20, point3(26, 3, 6), point3(0, 2, 0), vec3(0, 1, 0));

    // texture vector
    texture_vector tv = texture_vector(50, 50, 50, 0, 0, 4);

    // add sphere and ground
    scene.add_sphere(point3(0, -1000, 0), 1000, "diffuse", tv);
    scene.add_sphere(point3(0, 2, 0), 2, "diffuse", tv);

    // render
    cam.render(scene, "perlin-noise.ppm");
}

// volumes
void volume_rendering()
{
    // create scene
    world scene(true);

    // camera settings
    camera cam(400, 225, color(.21, .21, .21));
    cam.configure(20, point3(20, 3, 6), point3(0, 2, 0), vec3(0, 1, 0));

    // add smoke
    auto white = make_shared<diffuse>(color(0, 0,2.04));
    shared_ptr<hittable> smoke_sphere = make_shared<sphere>(point3(0, 1.5, 0), 2, white);
    
    scene.add_volume(smoke_sphere, .01, "volume", texture_vector(0, 0, 0));

    // render
    cam.render(scene, "volume.ppm");
}

#endif