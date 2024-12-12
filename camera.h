#ifndef CAMERA_H
#define CAMERA_H

// header file for the camera class that actual renders the world and creates the ppm file

// include
#include "utility.h"
#include "material.h"

// camera
class camera
{
public:
    // basic camera settings
    int image_width, image_height;
    color background;

    // configurable settings
    point3 center;
    bool is_configured = false;

    // camera constructor to set the width, height, and background color
    camera(int width = 400, int height = 225, color bg = color(0.70, 0.80, 1.00))
    {
        image_width = width;
        image_height = height;
        background = bg;
    }

    // function to configure the camera
    void configure(double vfov, point3 look_from, point3 target, vec3 vup)
    {
        vec3 u, v, w;
        is_configured = true;
        center = look_from;

        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * 10;
        auto viewport_width = viewport_height * (double(image_width) / image_height);

        w = unit_vector(center - target);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        vec_del_u = (viewport_width * u) / image_width;
        vec_del_v = (viewport_height * -v) / image_height;

        auto viewport_upper_left = center - (10 * w) - (viewport_width * u) / 2 - (viewport_height * -v) / 2;
        upper_left_pixel = viewport_upper_left + 0.5 * (vec_del_u + vec_del_v);
    }

    // function to render the world to output file
    void render(const hittable &world, std::string output_filename, bool anti = true)
    {
        // make sure camera is configured
        if (!is_configured)
        {
            std::cerr << "Camera Not Configured" << std::endl;
            return;
        }

        // path to render
        std::string path = "Renders";
        std::string filepath = path + "/" + output_filename;

        // open the file for writing
        std::ofstream outFile(filepath);
        if (!outFile)
        {
            std::cerr << "Error: Could not open the file for writing!" << std::endl;
            return;
        }

        // PPM header
        outFile << "P3\n"
                << image_width << ' ' << image_height << "\n255\n";

        // double for loop to go over all image pixels
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
                        pixel_color += ray_color(r, 50, world);
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
    vec3 vec_del_u, vec_del_v;
    point3 upper_left_pixel;
    
    // get the ray of the given pixel
    ray get_ray(int i, int j) const
    {
        auto offset = vec3(random_double() - 0.5, random_double() - 0.5, 0);
        auto pixel_sample = upper_left_pixel + ((i + offset.x()) * vec_del_u) + ((j + offset.y()) * vec_del_v);

        auto ray_origin = center;
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }

    // function to get the ray color
    color ray_color(const ray &r, int depth, const hittable &world) const
    {
        ray scattered;
        color attenuation;

        if (depth <= 0)
        {
            return color(0, 0, 0);
        }

        place_hit rec;

        if (!world.intersect(r, interval(0.001, infinity), rec))
        {
            return background;
        }

        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
        {
            return color_from_emission;
        }

        color color_from_scatter = attenuation * ray_color(scattered, depth - 1, world);

        return color_from_emission + color_from_scatter;
    }

    // function to print progress bar
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

    // write the color to the PPM file
    void write_color(std::ostream &out, const color &pixel_color)
    {
        auto r = pixel_color.x();
        auto g = pixel_color.y();
        auto b = pixel_color.z();

        r = (r > 0) ? std::sqrt(r) : 0;
        g = (g > 0) ? std::sqrt(g) : 0;
        b = (b > 0) ? std::sqrt(b) : 0;

        static const interval intensity(0.000, 0.999);
        int rbyte = int(256 * intensity.clamp(r));
        int gbyte = int(256 * intensity.clamp(g));
        int bbyte = int(256 * intensity.clamp(b));

        out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
    }
};

#endif