#include "sphere.hpp"

Sphere::Sphere(const Point3& center, float radius, std::shared_ptr<Material> mat)
            : center(center), radius(std::fmax(0, radius)), mat(mat) {
    // Initialize bounding box
    Vec3 rvec = Vec3(radius, radius, radius);
    bbox = Aabb(center - rvec, center + rvec);
}

bool Sphere::hit(const Ray& r, Interval rayT, HitRecord& rec) const {
    /*
    C sphere center, r ray
    O ray origin, d direction

    P(t) = O + td
    P(t) is on sphere if (C-P(t))⋅(C-P(t)) = r^2
    which means
    (t^2)d⋅d − 2td⋅(C−O) + (C−O)⋅(C−O) − r^2 = 0

    h = -b/2 = d⋅(C−O)
    */
    Vec3 oc = center - r.origin();
    Vec3 d = r.direction();
    float a = glm::dot(d, d);
    float h = glm::dot(d, oc);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = h * h - a * c;

    if (discriminant < 0) return false;

    // Find the nearest root that lies in the acceptable range
    float sqrtd = sqrt(discriminant);
    float root = (h - sqrtd) / a;
    if (!rayT.surrounds(root)) {
        root = (h + sqrtd) / a;
        if (!rayT.surrounds(root)) {
            return false;
        }
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    Vec3 outwardNormal = (rec.p - center) / radius;
    rec.setFaceNormal(r, outwardNormal);
    getSphereUV(outwardNormal, rec.u, rec.v);
    rec.material = mat;
    return true;
}

void Sphere::getSphereUV(const Point3& p, float& u, float& v) {
    // p: a given point on the sphere of radius one, centered at the origin
    // u: returned value [0,1] of angle around the Y axis from -X to +Z to +X to -Z back to -X
    // v: returned value [0,1] of angle from -Y to +Y
    //     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
    //     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
    //     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

    float theta = std::acos(-p.y);
    float phi = std::atan2(-p.z, p.x) + pi;

    u = phi / (2 * pi);
    v = theta / pi;
}