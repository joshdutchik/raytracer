#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H

// header file to process the give image and use the stb_image header file to process a custom texture

// include
#include "utility.h"
#include "stb_image.h"

// image_loader
class image_loader
{
public:
    // constructor for image loader
    image_loader() {}

    // constructor to create an image loader and load an image file using the file name
    image_loader(const char *image_filename)
    {
        auto filename = std::string(image_filename);

        if (load(filename))
        {
            return;
        }

        debugger::getInstance().logToFile("Could not load image file");
    }

    // get image width
    int width() const
    {
        if (fdata == nullptr)
        {
            return 0;
        }

        return image_width;
    }

    // get image height
    int height() const
    {
        if (fdata == nullptr)
        {
            return 0;
        }

        return image_height;
    }

    // function to load the image file using stb
    bool load(const std::string &filename)
    {
        auto n = bytes_per_pixel;
        fdata = stbi_loadf(filename.c_str(), &image_width, &image_height, &n, bytes_per_pixel);

        if (fdata == nullptr)
        {
            return false;
        }

        bytes_per_scanline = image_width * bytes_per_pixel;
        convert_to_bytes();

        return true;
    }

    // get the pixel data of the x,y coordinate of the image
    const unsigned char *pixel_data(int x, int y) const
    {
        static unsigned char magenta[] = {255, 0, 255};

        if (bdata == nullptr)
        {
            return magenta;
        }

        x = clamp(x, 0, image_width);
        y = clamp(y, 0, image_height);

        return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
    }

private:
    const int bytes_per_pixel = 3;
    int image_width = 0, image_height = 0, bytes_per_scanline = 0;
    float *fdata = nullptr;
    unsigned char *bdata = nullptr;

    // some helper functions
    static int clamp(int x, int low, int high)
    {
        if (x < low)
        {
            return low;
        }

        if (x < high)
        {
            return x;
        }

        return high - 1;
    }

    static unsigned char float_to_byte(float value)
    {
        if (value <= 0.0)
        {
            return 0;
        }

        if (1.0 <= value)
        {
            return 255;
        }

        return static_cast<unsigned char>(256.0 * value);
    }

    void convert_to_bytes()
    {
        int total_bytes = image_width * image_height * bytes_per_pixel;
        bdata = new unsigned char[total_bytes];

        auto *bptr = bdata;
        auto *fptr = fdata;

        for (auto i = 0; i < total_bytes; i++, fptr++, bptr++)
        {
            *bptr = float_to_byte(*fptr);
        }
    }
};

#endif