#include "myPT.hpp"
#include "camera.hpp"
#include "hittableList.hpp"
#include "scenes.hpp"

using namespace comUtils;

void chooseCameraAndScene(Camera& cam, HittableList& scene){
    int sceneNum = ptInput::readSceneNumber(INPUT_FILE);
    switch (sceneNum) {
        case 0:
        default: {
            // EXTERNAL 3D MODEL
            cam = ptInput::setUpCamera(INPUT_FILE);
            std::string modelPath = input::readModelName(INPUT_FILE);
            std::string temp = modelPath;
            modelPath = "models/" + temp + "/" + temp + ".obj"; 
            scene = HittableList(ptScenes::externalModel(modelPath));
            break;
        } case 1:
            // RAY TRACING IN ONE WEEKEND SPHERES
            cam = Camera(16.0f/9.0f, 1920, 20, Vec3(13,2,3), Vec3(0,0,0));
            cam.setImageName(input::readOutputImageName(INPUT_FILE));
            cam.setSamplesPerPixel(10);
            cam.setMaxDepth(50);
            cam.setDefocusAngle(0.6);
            cam.setFocusDist(10.0);
            cam.setBackground(Color(0.70, 0.80, 1.00));
            scene = HittableList(ptScenes::oneWeekendSpheres());
            break; 
        case 2:
            // CORNELL BOX
            cam = Camera(1, 800, 40, Point3(278,278,-800), Point3(278,278,0));
            cam.setImageName(input::readOutputImageName(INPUT_FILE));
            cam.setSamplesPerPixel(10000);
            cam.setMaxDepth(7);
            scene = HittableList(ptScenes::cornellBox());
            break;
        case 3:
            // MIRROR ROOM
            //cam = Camera(1, 100, 60, Point3(400,400,50), Point3(250,250,700));
            cam = Camera(1, 1000, 45, Point3(100,300,550), Point3(305,300,75));
            cam.setImageName(input::readOutputImageName(INPUT_FILE));
            cam.setSamplesPerPixel(1000);
            cam.setMaxDepth(40);
            scene = HittableList(ptScenes::mirrorRoom());
            break;
    }
}

void renderScene(Camera cam, const Hittable& scene){
    auto start = std::chrono::high_resolution_clock::now();
    cam.render(scene);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
    unsigned int seconds = duration.count();
    std::clog << "Rendering time: " << seconds/3600 << "h "
              << (seconds/60)%60 << "m " << seconds%60 << "s\n";
}

int main() {
    std::clog << "Running " << ptInput::readNumThreads(INPUT_FILE)
              << " threads, with " << ptInput::readNumSubImages(INPUT_FILE)
              << " sub-images to be rendered\n\n";
    Camera cam;
    HittableList scene;
    chooseCameraAndScene(cam, scene);
    renderScene(cam, scene);
    return 0;
}
