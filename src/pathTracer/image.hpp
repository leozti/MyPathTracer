#pragma once

#include "myPT.hpp"

class Image {
    public:
        Image() {}

        // Loads image data from the specified file path
        Image(const std::string& filePath);

        ~Image();

        // Loads the linear (gamma=1) image data from the given file name.
        // Returns true if the load succeeded.
        // The resulting data buffer contains the three [0.0, 1.0] floating-point values
        // for the each pixel (red, green, blue).
        // Pixels are contiguous, going left to right for the width of the image, followed
        // by the next row below, for the full height of the image.
        bool load(const std::string& filePath);

        int width()  const { return (fdata == nullptr) ? 0 : imageWidth; }
        int height() const { return (fdata == nullptr) ? 0 : imageHeight; }

        // Return the address of the three RGB bytes of the pixel at (x,y).
        // If there is no image data, returns magenta.
        const unsigned char* pixelData(int x, int y) const;

        const std::string& getFilePath(){return filePath;}

    private:
        std::string filePath;
        const int bytesPerPixel = 3;
        // Linear floating point pixel data
        float *fdata = nullptr;
        // Linear 8-bit pixel data
        unsigned char *bdata = nullptr;
        // Loaded image width
        int imageWidth = 0;
        // Loaded image height
        int imageHeight = 0;
        int bytesPerScanline = 0;

        static int clamp(int x, int low, int high);

        static unsigned char floatToByte(float value);

        // Convert the linear floating point pixel data to bytes, storing the resulting
        // byte data in the `bdata` member.
        void convertToBytes();
};