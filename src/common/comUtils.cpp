#include "comUtils.hpp"

using std::string;
using std::ifstream;
using std::istringstream;

namespace comUtils {

void fatalError (std::string message) {
    std::cerr << message << std::endl;
    exit(EXIT_FAILURE);
}

namespace input {

string readModelName(const string& inputFileName) {
    return details::readParameterAt<string>(inputFileName, 5);
}

string readOutputImageName(const string& inputFileName) {
    return details::readParameterAt<string>(inputFileName, 9);
}

CamSettings readCameraSettings(const std::string& inputFileName) {    
    using details::skipLines, details::readParameter;
    CamSettings camSet;

    ifstream inputFile;
    inputFile.open(INPUT_FILE);
    if (inputFile.fail()) fatalError("Error: failed opening input file");

    skipLines(inputFile, 6);

    // Image name
    skipLines(inputFile, 2);
    camSet.imageName = readParameter<string>(inputFile);
    
    // Aspect Ratio
    skipLines(inputFile, 2);
    float width = readParameter<float>(inputFile);
    float height = readParameter<float>(inputFile);
    camSet.aspectRatio = width / height;

    // Image Width
    skipLines(inputFile, 1);
    camSet.imageWidth = readParameter<int>(inputFile);

    // Vertical Field of View
    skipLines(inputFile, 1);
    camSet.vfov = readParameter<float>(inputFile);

    float x, y, z;
    // "Look-From" Vector
    skipLines(inputFile, 2);
    x = readParameter<float>(inputFile);
    y = readParameter<float>(inputFile);
    z = readParameter<float>(inputFile);
    camSet.lookFrom = glm::vec3(x, y, z);

    // "Look-At" Vector
    skipLines(inputFile, 2);
    x = readParameter<float>(inputFile);
    y = readParameter<float>(inputFile);
    z = readParameter<float>(inputFile);
    camSet.lookAt = glm::vec3(x, y, z);

    // "Up" Vector
    skipLines(inputFile, 2);
    x = readParameter<float>(inputFile);
    y = readParameter<float>(inputFile);
    z = readParameter<float>(inputFile);
    camSet.up = glm::vec3(x, y, z);

    // Per-Pixel Samples
    skipLines(inputFile, 1);
    camSet.samplesPerPixel = readParameter<int>(inputFile);

    // Defocus Angle
    skipLines(inputFile, 1);
    camSet.defocusAngle = readParameter<float>(inputFile);

    // Focus Distance
    skipLines(inputFile, 1);
    camSet.focusDist = readParameter<float>(inputFile);

    // Max Depth of Ray Bounces
    skipLines(inputFile, 1);
    camSet.maxDepth = readParameter<int>(inputFile);

    // Background Color
    skipLines(inputFile, 2);
    x = readParameter<float>(inputFile);
    y = readParameter<float>(inputFile);
    z = readParameter<float>(inputFile);
    camSet.background = glm::vec3(x, y, z);

    return camSet;
}

void details::skipLines(ifstream& inputStream, int n) {
    string line;
    for (int i = 0; i < n; i++) {
        getline(inputStream, line);
    }
}

} // namespace input

namespace materials {

matType determineMatType(aiMaterial *material, illModel model){
    if (isDiffuseLight(material, model)) { return DIFFUSE_LIGHT; }
    switch (model) {
        case BASE_SHADING:
        default:
            return LAMBERTIAN;
        case REFLECTION_ON:
            return METAL;
        case REFRACTION_ON:
            return DIELECTRIC; 
    }
}

bool isDiffuseLight(aiMaterial *material, illModel model){
    bool isEmissive = false;
    aiColor3D color;
    material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
    if ((model == BASE_SHADING)&&(color.r != 0 || color.g != 0 || color.b != 0)) {
        isEmissive = true;
    }
    return isEmissive;
}

illModel readIllModel(const string& materialName, const string& mtlFilePath){
    ifstream mtlFile;
    mtlFile.open(mtlFilePath);
    if (mtlFile.fail()) fatalError("Error: failed opening file " + mtlFilePath);

    // find material description
    string line;
    do {
        getline(mtlFile, line);
    } while (line != "newmtl " + materialName);
    // read illumination model
    string s;
    do {
        mtlFile >> s;
    } while (s != "illum");
    int n;
    mtlFile >> n;
    return static_cast<illModel>(n); 
}

} // namespace materials

} // namespace comUtils