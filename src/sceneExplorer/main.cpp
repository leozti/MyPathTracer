#define STB_IMAGE_IMPLEMENTATION
#include "mySceneExp.hpp"

#include "window.hpp"
#include "shader.hpp"
#include "mesh.hpp"
#include "camera.hpp"
#include "model.hpp"
#include "utilities.hpp"

using std::string;
using glm::vec3, glm::mat4;

using namespace comUtils;

///////////////////////////// GLOBAL VARIABLES /////////////////////////////

input::CamSettings camSet = input::readCameraSettings(INPUT_FILE);
Camera camera(camSet.lookFrom);
bool cameraIsMoving[NUM_DIRECTIONS];

Window window(camSet.aspectRatio, camSet.imageName);

float focusPlaneDistance = camSet.focusDist;
// turn focus plane on/off
bool focusPlaneOn = false;

// timing
float deltaTime = 0.0f;  // time between current frame and last frame
float lastFrame = 0.0f;

/////////////////////////// USER INPUT FUNCTIONS ///////////////////////////

CamMovement keyToMovement(SDL_Keycode key) {
    switch (key) {
        case SDLK_w:
            return FORWARD;
        case SDLK_s:
            return BACKWARD;
        case SDLK_a:
            return LEFT;
        case SDLK_d:
            return RIGHT;
        case SDLK_SPACE:
            return UP;
        case SDLK_e:
            return DOWN;
    }
    return NO_MOVEMENT;
}

void processKeyDown(SDL_Keycode key) {
    switch (key) {
    case SDLK_ESCAPE:
        window.close();
        break;
    case SDLK_RETURN:
        seOutput::writeOutput(camera, focusPlaneDistance);
        window.close();
        break;
    case SDLK_f:
        focusPlaneOn = !focusPlaneOn;
        break;
    case SDLK_UP:
        focusPlaneDistance += 0.1f;
        break;
    case SDLK_DOWN:
        focusPlaneDistance -= 0.1f;
        break;
    default: {
        CamMovement mvmt = keyToMovement(key);
        if (mvmt != NO_MOVEMENT) cameraIsMoving[mvmt] = true;
        }
    }
}

void processInput(){
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_MOUSEMOTION:
                camera.processMouseMovement(e.motion.xrel, -e.motion.yrel);
                break;
            case SDL_KEYDOWN:
                processKeyDown(e.key.keysym.sym);
                break;
            case SDL_KEYUP: {
                CamMovement mvmt = keyToMovement(e.key.keysym.sym);
                if (mvmt == NO_MOVEMENT) break;
                cameraIsMoving[mvmt] = false;
                break;
            }
        }
    }
}

void updateCamera () {
    bool noMovement = true;
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        if (cameraIsMoving[i] == true) {
            noMovement = false;
            break;
        }    
    }
    if (noMovement) return;
    camera.move(cameraIsMoving, deltaTime);
}

/////////////////////////////////// MAIN ///////////////////////////////////

int main() {
    // Initialize shader program
    Shader ourShader(VERTEX_SHADER, FRAGMENT_SHADER);

    #ifdef FLIP_Y_AXIS_TEXTURE
    stbi_set_flip_vertically_on_load(true);
    #endif
    
    // Load 3D model
    string modelName = comUtils::input::readModelName(INPUT_FILE);
    string modelObjPath = "models/" + modelName + "/" + modelName + ".obj";
    Model ourModel(modelObjPath);

    // Build focus plane
    Vertex v0, v1, v2, v3;
    // Coordinates are already expressed in the camera's coordinate system
    vec3 x(1.0f,0.0f,0.0f); vec3 y(0.0f,1.0f,0.0f);
    v0.position = (-x +y)*FOCUS_PLANE_SIZE;
    v1.position = (-x -y)*FOCUS_PLANE_SIZE;
    v2.position = (+x +y)*FOCUS_PLANE_SIZE;
    v3.position = (+x -y)*FOCUS_PLANE_SIZE;
    std::vector<Vertex> vertices = {v0,v1,v2,v3};
    std::vector<unsigned int> indices = {0,1,2,3,2,1};
    Mesh focusPlane(vertices, indices, materials::LAMBERTIAN);
    focusPlane.setColor(FOCUS_PLANE_COLOR);

    // Model-View-Projection Matrices
    MVP mvp;
    mvp.projection = glm::perspective(glm::radians(camSet.vfov),camSet.aspectRatio,1.0f,1000.0f);
    MVP planeMVP;
    planeMVP.projection = mvp.projection;

    // Enable z-buffering and backface culling
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);

    // Enable alpha blending
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ourShader.use();

    // Camera starts still
    for (int i = 0; i < NUM_DIRECTIONS; i++) {
        cameraIsMoving[i] = false;
    }

    glm::vec3 background = camSet.background;
    // Rendering loop
    while(!window.isClosed()){
        // per-frame time logic
        float currentFrame = SDL_GetTicks();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        // Process user input
        processInput();

        // Move around camera
        updateCamera();
        mvp.view = camera.getViewMatrix();
        updateMVP(ourShader, mvp);

        // Draw to the screen
        window.clear(background.r,background.g,background.b,1.0f);        
        ourModel.draw(ourShader);

        if (focusPlaneOn) {
            planeMVP.model = glm::translate(glm::vec3(0.0f,0.0f,-focusPlaneDistance));
            updateMVP(ourShader, planeMVP);
            focusPlane.draw(ourShader);    
        }

        // Show new rendered image
        window.update();
    }
    return 0;
}

