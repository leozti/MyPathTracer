#pragma once

#include "myPT.hpp"

#include "hittable.hpp"
#include "ray.hpp"
#include "interval.hpp"
#include "aabb.hpp"

class Sphere : public Hittable {
    public:
        Sphere(const Point3& center, float radius, std::shared_ptr<Material> mat);

        bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override;

        Aabb boundingBox() const override {return bbox;}
        
    private:
        static void getSphereUV(const Point3& p, float& u, float& v);
        Point3 center;
        float radius;
        std::shared_ptr<Material> mat;
        Aabb bbox;
};