#pragma once

#include "myPT.hpp"

class Interval {
    public:
        float min, max;

        // Default interval is empty
        Interval() : min(+infinity), max(-infinity) {}
        
        Interval(float min, float max) : min(min), max(max) {}

        // Create the interval tightly enclosing the two input intervals
        Interval(const Interval& a, const Interval& b) {
            min = a.min <= b.min ? a.min : b.min;
            max = a.max >= b.max ? a.max : b.max;
        }
        
        // Returns max - min
        float size() const { return max - min; }
        
        bool contains(float x) const { return min <= x && x <= max; }
        
        bool surrounds(float x) const { return min < x && x < max; }
        
        float clamp(float x) const {
            if (x < min) return min;
            if (x > max) return max;
            return x;
        }

        Interval expand(float delta) const {
            float padding = delta/2;
            return Interval(min - padding, max + padding);
        }

        static const Interval empty, universe;
};