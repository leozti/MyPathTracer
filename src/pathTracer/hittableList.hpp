#pragma once

#include "myPT.hpp"

#include "hittable.hpp"
#include "interval.hpp"
#include "aabb.hpp"

class HittableList : public Hittable {
    public:
        std::vector<std::shared_ptr<Hittable>> objects;
        
        HittableList() {}
        
        HittableList(std::shared_ptr<Hittable> object) { add(object); }
        
        void clear() { objects.clear(); }
        
        void add(std::shared_ptr<Hittable> object) {
            objects.push_back(object);
            bbox = Aabb(bbox, object->boundingBox());
        }
        
        bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
            HitRecord tempRec;
            bool hitAnything = false;
            float closestSoFar = rayT.max;
            for (const auto& object : objects) {
                if (object->hit(r, Interval(rayT.min, closestSoFar), tempRec)) {
                    hitAnything = true;
                    closestSoFar = tempRec.t;
                    rec = tempRec;
                }
            }
            return hitAnything;
        }

        Aabb boundingBox() const override { return bbox; }

    private:
        Aabb bbox;      
};