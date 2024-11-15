#include "myPT.hpp"
#include "hittable.hpp"
#include "sphere.hpp"
#include "material.hpp"
#include "bvh.hpp"
#include "triangle.hpp"
#include "texture.hpp"

#include "model.hpp"

namespace ptScenes {
    std::shared_ptr<Hittable> externalModel(const std::string& objFilepath);
    
    std::shared_ptr<Hittable> oneWeekendSpheres();

    std::shared_ptr<Hittable> cornellBox();

    std::shared_ptr<Hittable> mirrorRoom();
};
