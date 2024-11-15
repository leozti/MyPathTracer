#pragma once

#include "myPT.hpp"
#include "ray.hpp"
#include "interval.hpp"
#include "aabb.hpp"

// needed to solve dependencies
class Aabb;
class Material;

struct HitRecord {
    Point3 p;
    Vec3 normal;
    std::shared_ptr<Material> material;
    float t;
    // u,v texture coordinates
    float u;
    float v;
    bool frontFace;
    // Sets the hit record normal vector
    // NOTE: the parameter `outwardNormal` is assumed to have unit length
    void setFaceNormal(const Ray& r, const Vec3& outwardNormal) {
        frontFace = glm::dot(r.direction(), outwardNormal) < 0;
        normal = frontFace ? outwardNormal : -outwardNormal;
    }
};

class Hittable {
    public:
        virtual ~Hittable() = default;
        virtual bool hit(const Ray& r, Interval rayT, HitRecord& rec) const = 0;
        virtual Aabb boundingBox() const = 0;
};