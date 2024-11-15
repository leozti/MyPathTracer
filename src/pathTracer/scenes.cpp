#include "scenes.hpp"

using std::shared_ptr;
using std::make_shared;

shared_ptr<Hittable> ptScenes::externalModel(const std::string& objFilePath) {
    Model model(objFilePath);
    model.initialize();

    unsigned int nMeshes = model.numberOfMeshes();
    std::clog << "Number of meshes in scene: " << nMeshes << "\n";
    unsigned int totTriangles = 0;
    for (unsigned int i = 0; i < nMeshes; i++) { 
        unsigned int nTriangles = model.getMesh(i).numberOfTriangles();
        std::clog << "Mesh #" << i+1 << " - Triangles: " << nTriangles << "\n";
        totTriangles += nTriangles;
    }
    std::clog << "Total number of triangles in scene: " << totTriangles << "\n\n";

    return model.buildBvh();
}

shared_ptr<Hittable> ptScenes::oneWeekendSpheres() {
    HittableList scene;
    auto groundMaterial = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    scene.add(make_shared<Sphere>(Point3(0,-1000,0), 1000, groundMaterial));
    
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto chooseMat = randomFloat();
            Point3 center(a + 0.9*randomFloat(), 0.2, b + 0.9*randomFloat());   
            if (glm::length(center - Point3(4, 0.2, 0)) > 0.9) {
                shared_ptr<Material> sphereMaterial;
                if (chooseMat < 0.8) {
                    // diffuse
                    Color albedo = randomVec3() * randomVec3();
                    sphereMaterial = make_shared<Lambertian>(albedo);
                    scene.add(make_shared<Sphere>(center, 0.2, sphereMaterial));    
                } else if (chooseMat < 0.95) {
                    // metal
                    Color albedo = randomVec3(0.5, 1);
                    float fuzz = randomFloat(0, 0.5);
                    sphereMaterial = make_shared<Metal>(albedo, fuzz);
                    scene.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                } else {
                    // glass
                    sphereMaterial = make_shared<Dielectric>(1.5);
                    scene.add(make_shared<Sphere>(center, 0.2, sphereMaterial));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    scene.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));
    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    scene.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));
    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    scene.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return make_shared<BvhNode>(scene);
}

shared_ptr<Hittable> ptScenes::cornellBox() {
    HittableList scene;
    auto red   = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

    Vertex v0; v0.position = Point3(555,0,0);
    Vertex v1; v1.position = Point3(555,555,0);
    Vertex v2; v2.position = Point3(555,0,555);
    Vertex v3; v3.position = Point3(555,555,555);
    Vertex v4; v4.position = Point3(0,0,0);
    Vertex v5; v5.position = Point3(0,555,0);
    Vertex v6; v6.position = Point3(0,0,555);
    Vertex v7; v7.position = Point3(0,555,555);
    Vec3 leftWallNormal = Vec3(-1,0,0);
    Vec3 rightWallNormal = Vec3(1,0,0);
    Vec3 floorNormal = Vec3(0,1,0);
    Vec3 ceilingNormal = Vec3(0,-1,0);
    Vec3 backWallNormal = Vec3(0,0,-1);

    // left wall
    v0.normal = v1.normal = v2.normal = v3.normal = leftWallNormal;
    scene.add(make_shared<Triangle>(v0, v1, v2, green));
    scene.add(make_shared<Triangle>(v1, v2, v3, green));
    // right wall
    v4.normal = v5.normal = v6.normal = v7.normal = rightWallNormal;
    scene.add(make_shared<Triangle>(v4, v5, v6, red));
    scene.add(make_shared<Triangle>(v5, v6, v7, red));
    // floor
    v4.normal = v0.normal = v6.normal = v2.normal = floorNormal;
    scene.add(make_shared<Triangle>(v4, v0, v6, white));
    scene.add(make_shared<Triangle>(v0, v6, v2, white));
    // ceiling
    v3.normal = v7.normal = v1.normal = v5.normal = ceilingNormal;
    scene.add(make_shared<Triangle>(v3, v7, v1, white));
    scene.add(make_shared<Triangle>(v7, v1, v5, white));
    // back wall
    v6.normal = v2.normal = v7.normal = v3.normal = backWallNormal;
    scene.add(make_shared<Triangle>(v6, v2, v7, white));
    scene.add(make_shared<Triangle>(v2, v7, v3, white));
    
    // light
    Vertex v8, v9, v10, v11;
    v8.position = Point3(343, 554, 332);
    v9.position = Point3(213, 554, 332);
    v10.position = Point3(343, 554, 227);
    v11.position = Point3(213, 554, 227);
    scene.add(make_shared<Triangle>(v8, v9, v10, light));
    scene.add(make_shared<Triangle>(v9, v10, v11, light));

    // spheres
    scene.add(make_shared<Sphere>(Point3(400,82.5,335), 82.5, make_shared<Metal>(Color(1,1,1), 0)));
    scene.add(make_shared<Sphere>(Point3(150,82.5,150), 82.5, make_shared<Dielectric>(1.5)));

    return make_shared<BvhNode>(scene);
}

shared_ptr<Hittable> ptScenes::mirrorRoom() {
    HittableList scene;
    auto mirror = make_shared<Metal>(Color(.93,.93,.93), 0.0);
    auto white = make_shared<Lambertian>(Color(0.88, 0.88, 0.88));
    auto light = make_shared<DiffuseLight>(Color(25, 25, 25));
    auto checker = make_shared<CheckerTexture>(50.0, Color(0.88, 0.88, 0.88), Color(0.0, 0.0, 0.0));

    Vertex v0; v0.position = Point3(555,0,0);
    Vertex v1; v1.position = Point3(555,555,0);
    Vertex v2; v2.position = Point3(555,0,555);
    Vertex v3; v3.position = Point3(555,555,555);
    Vertex v4; v4.position = Point3(0,0,0);
    Vertex v5; v5.position = Point3(0,555,0);
    Vertex v6; v6.position = Point3(0,0,555);
    Vertex v7; v7.position = Point3(0,555,555);
    Vec3 leftWallNormal = Vec3(-1,0,0);
    Vec3 rightWallNormal = Vec3(1,0,0);
    Vec3 floorNormal = Vec3(0,1,0);
    Vec3 ceilingNormal = Vec3(0,-1,0);
    Vec3 backWallNormal = Vec3(0,0,-1);
    Vec3 frontWallNormal = Vec3(0,0,1);

    // floor
    v4.normal = v0.normal = v6.normal = v2.normal = floorNormal;
    scene.add(make_shared<Triangle>(v0, v2, v4, make_shared<Lambertian>(checker)));
    scene.add(make_shared<Triangle>(v2, v6, v4, make_shared<Lambertian>(checker)));
    // left wall
    v0.normal = v1.normal = v2.normal = v3.normal = leftWallNormal;
    scene.add(make_shared<Triangle>(v0, v1, v2, white));
    scene.add(make_shared<Triangle>(v1, v2, v3, white));
    // right wall
    v4.normal = v5.normal = v6.normal = v7.normal = rightWallNormal;
    scene.add(make_shared<Triangle>(v4, v5, v6, white));
    scene.add(make_shared<Triangle>(v5, v6, v7, white));
    // ceiling
    v3.normal = v7.normal = v1.normal = v5.normal = ceilingNormal;
    scene.add(make_shared<Triangle>(v3, v7, v1, white));
    scene.add(make_shared<Triangle>(v7, v1, v5, white));
    // back wall
    v6.normal = v2.normal = v7.normal = v3.normal = backWallNormal;
    scene.add(make_shared<Triangle>(v6, v2, v7, mirror));
    scene.add(make_shared<Triangle>(v2, v7, v3, mirror));
    // front wall
    v4.normal = v0.normal = v1.normal = v5.normal = frontWallNormal;
    scene.add(make_shared<Triangle>(v4, v0, v1, mirror));
    scene.add(make_shared<Triangle>(v4, v1, v5, mirror));
    
    // light
    Vertex v8, v9, v10, v11;
    v8.position = Point3(343, 554, 332);
    v9.position = Point3(213,554,332);
    v10.position = Point3(343,554,227);
    v11.position = Point3(213, 554,227);
    scene.add(make_shared<Triangle>(v8, v9, v10, light));
    scene.add(make_shared<Triangle>(v9, v10, v11, light));

    // sphere
    auto sphereTexture = make_shared<ImageTexture>(
                         make_shared<Image>("images/textures/pexels.jpg"));
    auto sphereSurface = make_shared<Lambertian>(sphereTexture);
    scene.add(make_shared<Sphere>(Point3(278,278,278), 40, sphereSurface));

    return make_shared<BvhNode>(scene);
}
