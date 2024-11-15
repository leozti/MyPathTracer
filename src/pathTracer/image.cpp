#include "image.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Image::Image(const std::string& filePath) : filePath(filePath) {
    // If the image was not loaded successfully, width() and height() will return 0.
    if (load(filePath)) return;
    fatalError("ERROR: Could not load image file");
}

Image::~Image() {
    delete[] bdata;
    STBI_FREE(fdata);
}

bool Image::load(const std::string& filePath) {
    this->filePath = filePath;
    int n;
    // n = original bytes per pixel in file
    // bytesPerPixel = desired bytes per pixel
    fdata = stbi_loadf(filePath.c_str(), &imageWidth, &imageHeight, &n, bytesPerPixel);
    if (fdata == nullptr) return false;

    bytesPerScanline = imageWidth * bytesPerPixel;
    convertToBytes();
    return true;
}

const unsigned char* Image::pixelData(int x, int y) const {
    static unsigned char magenta[] = {255, 0, 255};
    if (bdata == nullptr) return magenta;
    x = clamp(x, 0, imageWidth);
    y = clamp(y, 0, imageHeight);
    return bdata + y * bytesPerScanline + x * bytesPerPixel;
}

int Image::clamp(int x, int low, int high) {
    // Return the value clamped to the range [low, high)
    if (x < low) return low;
    if (x < high) return x;
    return high - 1;
}

unsigned char Image::floatToByte(float value) {
    if (value <= 0.0) return 0;
    if (1.0 <= value) return 255;
    return static_cast<unsigned char>(256.0 * value);
}

void Image::convertToBytes() {
    int totalBytes = imageWidth * imageHeight * bytesPerPixel;
    bdata = new unsigned char[totalBytes];
    // Iterate through all pixel components, converting from [0.0, 1.0] float values to
    // unsigned [0, 255] byte values.
    auto* bptr = bdata;
    auto* fptr = fdata;
    for (auto i = 0; i < totalBytes; i++, fptr++, bptr++) {
        *bptr = floatToByte(*fptr);
    }
}