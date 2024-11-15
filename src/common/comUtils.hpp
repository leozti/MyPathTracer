#pragma once

#include <iostream>
#include <fstream>

#include <glm/glm.hpp>
#include <assimp/scene.h>

// Input file name
#define INPUT_FILE "ptInput.txt"

// Common Utilities
namespace comUtils {

// Exits program and prints error message `message`
void fatalError (std::string message);

namespace input {      
    // Returns the name of the input 3D model specified in the input file
    std::string readModelName(const std::string& inputFileName);
    
    // Returns the output image's name, as specified in the input file
    std::string readOutputImageName(const std::string& inputFileName);

    struct CamSettings {
        float aspectRatio;
        int imageWidth;
        float vfov;
        glm::vec3 lookFrom;
        glm::vec3 lookAt;
        glm::vec3 up;
        std::string imageName;
        int samplesPerPixel;
        float defocusAngle;
        float focusDist;
        int maxDepth;
        glm::vec3 background;
    };

    // Reads camera settings from the input file
    CamSettings readCameraSettings(const std::string& inputFileName);

    namespace details {
        // skip `n` lines in `inputStream`
        void skipLines(std::ifstream& inputStream, int n);

        // Reads the parameter from current line and moves 
        // to the next line in `inputStream`
        template <typename T>
        T readParameter(std::ifstream& inputStream);

        // Opens input file and reads the value assigned to
        // the parameter at line `line`
        template <typename T>
        T readParameterAt(const std::string& inputFileName, unsigned int line);
    }
}

namespace materials {
    // Illumination Model
    enum illModel {
        BASE_SHADING = 2,
        REFLECTION_ON = 3,
        REFRACTION_ON = 6
    };
    // Material Type
    enum matType {
        LAMBERTIAN,
        METAL,
        DIELECTRIC,
        DIFFUSE_LIGHT
    };

    matType determineMatType(aiMaterial *material, illModel model);

    bool isDiffuseLight(aiMaterial *material, illModel model);

    // Reads illumination model for the specified material in .mtl file
    illModel readIllModel(const std::string& materialName, const std::string& mtlFilePath);
}

}

// Implementations of template functions (they need to be visible)
#include "comUtilsImpl.hpp"