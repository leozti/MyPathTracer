#pragma once

#include <math.h>

#include "myPT.hpp"
#include "interval.hpp"
#include "camera.hpp"

// Utility functions

// RANDOM NUMBERS

// Returns a random real of type float in [0,1]
float randomFloat();

// Returns a random real of type float in [min,max]
float randomFloat(float min, float max);

// Returns a random integer in [min,max]
int randomInt(int min, int max);

// VECTORS

// Return a random Vec3 with components in [0,1]
Vec3 randomVec3();

// Return a random Vec3 with components in [min,max]
Vec3 randomVec3(float min, float max);

// Returns a random vector inside the unit sphere 
Vec3 randomInUnitSphere();

Vec3 randomUnitVector();

// Returns a random unit vector on the hemisphere above `normal`
Vec3 randomOnHemisphere(const Vec3& normal);

// Returns a random vector inside the unit disk
Vec3 randomInUnitDisk();

// Returns true if the vector is close to zero in all dimensions
bool nearZero(Vec3 v);

// COLORS

// Applies a linear to gamma transform for gamma = 2
float linearToGamma(float linear);

void writeColor(std::ostream &out, const Color &pixelColor);

// CAMERA SETUP FROM INPUT FILE

// needed to solve dependencies
class Camera;

// Path Tracer Input utilities
namespace ptInput {
    // Returns scene number specified in the input file  
    int readSceneNumber(const std::string& inputFileName);

    Camera setUpCamera(const std::string& inputFileName);

    // Returns number of threads to be used, as specified in the input file  
    int readNumThreads(const std::string& inputFileName);

    // Returns number of sub-images to divide the output image in,
    // as specified in the input file  
    int readNumSubImages(const std::string& inputFileName);
}