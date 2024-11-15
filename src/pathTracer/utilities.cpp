#include "utilities.hpp"

using std::fabs;

float randomFloat() {
    return (float)rand() / (float)RAND_MAX;
}

float randomFloat(float min, float max) {
    return min + (max-min)*randomFloat();
}

int randomInt(int min, int max) {
    return min + (rand()%(max-min+1));
}

Vec3 randomVec3() {
    return Vec3(randomFloat(), randomFloat(), randomFloat());
}

Vec3 randomVec3(float min, float max) {
    return Vec3(randomFloat(min,max), randomFloat(min,max), randomFloat(min,max));
}

Vec3 randomInUnitSphere() {
    while (true) {
        Vec3 p = randomVec3(-1,1);
        if (glm::length(p) < 1){
            return p;
        }
    }
}

Vec3 randomUnitVector(){
    return glm::normalize(randomInUnitSphere());
}

Vec3 randomOnHemisphere(const Vec3& normal){
    Vec3 onUnitSphere = randomUnitVector();
    if (glm::dot(onUnitSphere, normal) < 0.0){
        onUnitSphere = -onUnitSphere;
    }
    return onUnitSphere;
}

Vec3 randomInUnitDisk() {
    while(true) {
        Vec3 v = Vec3(randomFloat(-1,1), randomFloat(-1,1), 0.0f);
        if (glm::length(v) < 1) return v;
    }
}

bool nearZero(Vec3 v){
    auto s = 1e-8;
    return ((fabs(v.x) < s) && (fabs(v.y) < s) && (fabs(v.z) < s));
}

float linearToGamma(float linear){
    if (linear > 0){
        return sqrt(linear);
    }
    return 0;
}

void writeColor(std::ostream &out, const Color &pixelColor) {
    float r = pixelColor.x;
    float g = pixelColor.y;
    float b = pixelColor.z;

    r = linearToGamma(r);
    g = linearToGamma(g);
    b = linearToGamma(b);

    // Translate values from range [0,1] to the byte range [0,255]
    static const Interval intensity(0.000f, 0.999f);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

using std::string, std::ifstream;
using namespace comUtils::input;

int ptInput::readSceneNumber(const string& inputFileName) {
    return details::readParameterAt<int>(inputFileName, 3);
}

Camera ptInput::setUpCamera(const string& inputFileName) {
    CamSettings camSet = readCameraSettings(INPUT_FILE);
    Camera cam(camSet.aspectRatio, camSet.imageWidth, camSet.vfov,
                camSet.lookFrom, camSet.lookAt, camSet.up);
    cam.setImageName(camSet.imageName);
    cam.setSamplesPerPixel(camSet.samplesPerPixel);
    cam.setDefocusAngle(camSet.defocusAngle);
    cam.setFocusDist(camSet.focusDist);
    cam.setMaxDepth(camSet.maxDepth);
    cam.setBackground(camSet.background);

    return cam;
}

int ptInput::readNumThreads(const std::string& inputFileName){
    return details::readParameterAt<int>(inputFileName, 49);
}

int ptInput::readNumSubImages(const std::string& inputFileName){
    return details::readParameterAt<int>(inputFileName, 51);
}