#pragma once

#include "myPT.hpp"
#include "ray.hpp"
#include "hittable.hpp"

struct Vertex {
    Point3 position;
    Vec3 normal;
    glm::vec2 texCoords;
    // Constructor with default member values
    Vertex()
        : position(Point3(0.0f,0.0f,0.0f)),
          normal(Point3(0.0f,1.0f,0.0f)),
          texCoords(glm::vec2(0.0f,0.0f)){}
};

class Triangle : public Hittable {
    public:
        Triangle(Vertex v0, Vertex v1, Vertex v2, std::shared_ptr<Material> mat);
        
        bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override;
        
        Aabb boundingBox() const override {return bbox;}
    
    private:
        // Triangle vertices
        Vertex v0;
        Vertex v1;
        Vertex v2;
        Vec3 e1; // v1 - v0
        Vec3 e2; // v2 - v0
        
        std::shared_ptr<Material> mat;
        Aabb bbox;

        void setBoundingBox();
};