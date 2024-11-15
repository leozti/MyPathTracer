#pragma once

#include "mySceneExp.hpp"
#include "shader.hpp"
#include "camera.hpp"

// MVP Utilies

// Model-View-Projection Matrix
struct MVP{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    glm::mat4 getMatrix(){
        return projection * view * model;
    }
};

// Update MVP Matrix in Vertex Shader.
// `mvp` members should already have meaningful values
void updateMVP(const Shader &program, MVP mvp);

// Scene Explorer Output utilities
namespace seOutput {
    // Writes current camera data on output file (same as input)
    void writeOutput(Camera camera, float focusPlaneDistance);

    namespace details {
        // Copies `n` lines from `inFile` to `outFile`
        void copyLines(std::ifstream& inFile, std::ofstream& outFile, unsigned int n);
        // Reads current vector from the `inFile` stream, and writes the new version `vec`
        // in `outFile` with the usual format
        void writeVector(std::ifstream& inFile, std::ofstream& outFile, glm::vec3 vec);
}
}