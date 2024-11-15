#pragma once

#include "myPT.hpp"

#include "aabb.hpp"
#include "hittable.hpp"
#include "hittableList.hpp"
#include "ray.hpp"
#include "interval.hpp"

#include <algorithm>

class BvhNode : public Hittable {
  public:
    BvhNode(HittableList list) : BvhNode(list.objects, 0, list.objects.size()) {
        // This constructor (without span indices) creates an implicit copy of the hittable list,
        // which we will modify. The lifetime of the copied list only extends until this constructor
        // exits. That's OK, because we only need the resulting bounding volume hierarchy to persist.
    }

    BvhNode(std::vector<std::shared_ptr<Hittable>>& objects, size_t start, size_t end) {
        // Build the bounding box of the span of source objects
        bbox = Aabb::empty;
        for (size_t objectIndex=start; objectIndex < end; objectIndex++) {
            bbox = Aabb(bbox, objects[objectIndex]->boundingBox());
        }

        int axis = bbox.longestAxis();

        auto comparator = (axis == 0) ? boxXCompare
                        : (axis == 1) ? boxYCompare
                                      : boxZCompare;

        size_t objectSpan = end - start;

        if (objectSpan == 1) {
            left = right = objects[start];
        } else if (objectSpan == 2) {
            left = objects[start];
            right = objects[start+1];
        } else {
            std::sort(std::begin(objects) + start, std::begin(objects) + end, comparator);

            size_t mid = start + objectSpan/2;
            left = std::make_shared<BvhNode>(objects, start, mid);
            right = std::make_shared<BvhNode>(objects, mid, end);
        }
    }

    bool hit(const Ray& r, Interval rayT, HitRecord& rec) const override {
        if (!bbox.hit(r, rayT)) return false;

        bool hitLeft = left->hit(r, rayT, rec);
        bool hitRight = right->hit(r, Interval(rayT.min, hitLeft ? rec.t : rayT.max), rec);

        return hitLeft || hitRight;
    }

    Aabb boundingBox() const override { return bbox; }

  private:
    std::shared_ptr<Hittable> left;
    std::shared_ptr<Hittable> right;
    Aabb bbox;

    static bool boxCompare(const std::shared_ptr<Hittable> a,
                           const std::shared_ptr<Hittable> b, int axisIndex) {
        auto aAxisInterval = a->boundingBox().axisInterval(axisIndex);
        auto bAxisInterval = b->boundingBox().axisInterval(axisIndex);
        return aAxisInterval.min < bAxisInterval.min;
    }

    static bool boxXCompare (const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
        return boxCompare(a, b, 0);
    }

    static bool boxYCompare (const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
        return boxCompare(a, b, 1);
    }

    static bool boxZCompare (const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b) {
        return boxCompare(a, b, 2);
    }
};
