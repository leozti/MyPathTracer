#include "aabb.hpp"

Aabb::Aabb(const Point3& a, const Point3& b) {
    x = (a.x <= b.x) ? Interval(a.x, b.x) : Interval(b.x, a.x);
    y = (a.y <= b.y) ? Interval(a.y, b.y) : Interval(b.y, a.y);
    z = (a.z <= b.z) ? Interval(a.z, b.z) : Interval(b.z, a.z);
    padToMinimums();
}

Aabb::Aabb(const Aabb& box0, const Aabb& box1) {
    x = Interval(box0.x, box1.x);
    y = Interval(box0.y, box1.y);
    z = Interval(box0.z, box1.z);
}

const Interval& Aabb::axisInterval(int n) const {
    if (n == 1) return y;
    if (n == 2) return z;
    return x;
}

bool Aabb::hit(const Ray& r, Interval rayT) const {
    const Point3& rayOrig = r.origin();
    const Vec3& rayDir = r.direction();

    for (int axis = 0; axis < 3; axis++) {
        /*
        P(t) = Q + t*d

        Intersection with planes x = x0 and x = x1:
        x0 = Qx + t0*dx
        x1 = Qx + t1*dx

        t0 = (x0 - Qx)/dx
        t1 = (x1 - Qx)/dx
        */
        const Interval& ax = axisInterval(axis);
        float dinv = 1.0f / rayDir[axis];

        float t0 = (ax.min - rayOrig[axis]) * dinv;
        float t1 = (ax.max - rayOrig[axis]) * dinv;

        if (t0 < t1) {
            if (t0 > rayT.min) rayT.min = t0;
            if (t1 < rayT.max) rayT.max = t1;
        } else {
            if (t1 > rayT.min) rayT.min = t1;
            if (t0 < rayT.max) rayT.max = t0;
        }

        if (rayT.max <= rayT.min) return false;
    }
    return true;
}

int Aabb::longestAxis() const {
    if (x.size() > y.size()) {
        return x.size() > z.size() ? 0 : 2;
    } else {
        return y.size() > z.size() ? 1 : 2;
    }
}

const Aabb Aabb::empty = Aabb(Interval::empty, Interval::empty, Interval::empty);
const Aabb Aabb::universe = Aabb(Interval::universe, Interval::universe, Interval::universe);

void Aabb::padToMinimums() {
    float delta = 0.0001;
    if (x.size() < delta) x = x.expand(delta);
    if (y.size() < delta) y = y.expand(delta);
    if (z.size() < delta) z = z.expand(delta);
}