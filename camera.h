#ifndef CAMERA_H
#define CAMERA_H

#include "utility.h"
#include "material.h"

class camera
{
public:
    // Basic Camera Settings
    int image_width; // Rendered image width in pixel count
    int image_height;
    color background; // Scene background color

    // Configurable Settings
    point3 center; // Camera Center
   
    // More Complexity Settings
    int max_depth = 50; // Maximum number of ray bounces into scene
    bool is_configured = false;

    // ??
    //double defocus_angle = 0; // Variation angle of rays through each pixel

    camera(int width = 400, int height = 225, color bg = color(0.70, 0.80, 1.00))
    {
        image_width = width;
        image_height = height;
        background = bg;
    }

    void configure(double vfov, point3 look_from, point3 target, vec3 vup)
    {
        vec3 u, v, w; // Camera frame basis vectors

        is_configured = true;
        center = look_from;

        // Determine viewport dimensions.
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * 10;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(center - target);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;   // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v; // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (10 * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        //auto defocus_radius = 10 * std::tan(degrees_to_radians(defocus_angle / 2));
        //defocus_disk_u = u * defocus_radius;
        //defocus_disk_v = v * defocus_radius;
    }

    void render(const hittable &world, std::string output_filename, bool anti = true)
    {
        // Make sure camera is configured
        if (!is_configured)
        {
            std::cerr << "Camera Not Configured" << std::endl;
            return;
        }

        std::string path = "Renders";
        std::string filepath = path + "/" + output_filename;

        // Open the file for writing
        std::ofstream outFile(filepath);
        if (!outFile)
        {
            std::cerr << "Error: Could not open the file for writing!" << std::endl;
            return;
        }

        outFile << "P3\n"
                << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++)
        {
            for (int i = 0; i < image_width; i++)
            {
                color pixel_color(0, 0, 0);
                int sample_count = (anti ? 100 : 1);

                for (int sample = 0; sample < sample_count; sample++)
                {
                    try
                    {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    catch (const std::exception &e)
                    {
                        debugger::getInstance().logToFile(e.what());
                    }
                }

                try
                {
                    write_color(outFile, (1.0 / sample_count) * pixel_color);
                }
                catch (const std::exception &e)
                {
                    debugger::getInstance().logToFile(e.what());
                }
            }
            print_progress_bar(j, image_height);
        }

        std::cout << "\n Render Completed in " << filepath << "\n";
    }

private:
    point3 pixel00_loc; // Location of pixel 0, 0
    vec3 pixel_delta_u; // Offset to pixel to the right
    vec3 pixel_delta_v; // Offset to pixel below
    // defocus
    //vec3 defocus_disk_u; // Defocus disk horizontal radius
    //vec3 defocus_disk_v; // Defocus disk vertical radius

    ray get_ray(int i, int j) const
    {
        // Construct a camera ray originating from the defocus disk and directed at a randomly
        // sampled point around the pixel location i, j.

        auto offset = vec3(random_double() - 0.5, random_double() - 0.5, 0);
        auto pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

        //auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }
/*
    vec3 sample_square() const
    {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }
/*
    point3 defocus_disk_sample() const
    {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }
    */

    color ray_color(const ray &r, int depth, const hittable &world) const
    {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
        {
            return color(0, 0, 0);
        }

        hit_record rec;

        // If the ray hits nothing, return the background color.
        if (!world.hit(r, interval(0.001, infinity), rec))
        {
            return background;
        }

        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;

        color color_from_scatter = attenuation * ray_color(scattered, depth - 1, world);

        return color_from_emission + color_from_scatter;
    }

    void print_progress_bar(int progress, int total, int bar_width = 100)
    {
        double ratio = static_cast<double>(progress) / (total - 1);
        int filled_length = static_cast<int>(ratio * bar_width);

        std::cout << "\r[";

        for (int i = 0; i < bar_width; ++i)
        {
            if (i < filled_length)
            {
                std::cout << "=";
            }

            else
            {
                std::cout << " ";
            }
        }

        std::cout << "] " << static_cast<int>(ratio * 100) << "%";
        std::cout.flush();
    }
};

#endif