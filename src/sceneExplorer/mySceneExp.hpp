#pragma once

// COMMON HEADER
// Common header for source files of scene explorer

// Libraries
#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <stb_image.h>

// Paths of Shaders
#define VERTEX_SHADER "src/sceneExplorer/sceneExp.vs"
#define FRAGMENT_SHADER "src/sceneExplorer/sceneExp.fs"

// Specifies whether the y-axis should be flipped when loading textures 
#define FLIP_Y_AXIS_TEXTURE

// Position of camera at start
const glm::vec3 CAMERA_START_POS(0.0f,0.0f,8.0f);

const float FOCUS_PLANE_SIZE = 10.0f;
const glm::vec4 FOCUS_PLANE_COLOR(0.7f,0.7f,0.7f,0.3f);

// Colors of special materials
const glm::vec4 METAL_COLOR = glm::vec4(0.6f,0.6f,0.6f,1.0f);
const glm::vec4 DIELECTRIC_COLOR = glm::vec4(0.0f,0.3f,0.8f,0.5f);
const glm::vec4 DIFFUSE_LIGHT_COLOR = glm::vec4(1.0f,1.0f,1.0f,1.0f);

// Utilities in common with path tracer
#include "../common/comUtils.hpp"
using comUtils::fatalError;