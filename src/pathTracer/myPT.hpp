#pragma once

// COMMON HEADER
// Common header for path tracer source files

// Libraries
#include <chrono>
#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <cstdlib>
#include <thread>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Directory for output file
#define OUTPUT_DIR "images"

// Specifies whether the y-axis should be flipped for image coordinates
// when reading from textures
#define FLIP_Y_AXIS_TEXTURE

// Vectors, points, colors
using Vec3 = glm::vec3;
using Point3 = Vec3; // (distinct names for geometric clarity)
using Color = Vec3;

// Constants
const float infinity = std::numeric_limits<float>::infinity();
const float pi = glm::pi<float>();

// Utilities that are shared with scene explorer
#include "../common/comUtils.hpp"
using comUtils::fatalError;