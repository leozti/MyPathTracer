#pragma once

#include "myPT.hpp"
#include "interval.hpp"
#include "ray.hpp"

class Aabb {
    public:
        Interval x, y, z;
        // The default AABB is empty, since intervals are empty by default
        Aabb() {}
        Aabb(const Interval& x, const Interval& y, const Interval& z)
            : x(x), y(y), z(z) {
            padToMinimums();
        }
        
        // `a` and `b` are the extrema of the bounding box
        Aabb(const Point3& a, const Point3& b);
        
        Aabb(const Aabb& box0, const Aabb& box1);

        bool hit(const Ray& r, Interval rayT) const;

        const Interval& axisInterval(int n) const;

        // Returns the index of the longest axis of the bounding box
        int longestAxis() const;

        static const Aabb empty, universe;
    private:
        // Adjusts the AABB so that no side is narrower than some delta,
        // padding if necessary
        void padToMinimums();
};