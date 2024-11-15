#include "triangle.hpp"

Triangle::Triangle(Vertex v0, Vertex v1, Vertex v2, std::shared_ptr<Material> mat)
        : v0(v0), v1(v1), v2(v2), mat(mat) {
    e1 = v1.position - v0.position;
    e2 = v2.position - v0.position;
    setBoundingBox();
}

bool Triangle::hit(const Ray& r, Interval rayT, HitRecord& rec) const {
    // Check if ray hits triangle using the Muller-Trumbore method
    Point3 p0 = v0.position;
    /*
    P intersection of ray on plane:
    P = O + td
    P = p0 + ue1 + ve2
    
    O + td = p0 + ue1 + ve2
    -td + ue1 + ve2 = O - p0
    */
    
    Vec3 pvec = glm::cross(r.direction(), e2);
    float det = glm::dot(pvec, e1);

    if (std::fabs(det) < 1e-8) return false; // triangle and ray are parallel
    
    float invDet = 1.0f / det;
    
    // u and v are barycentric coordinates:
    // P = (1-u-v)v0 + uv1 + vv2
    // 0 <= u,v <= 1
    // where P is a point on the triangle

    Vec3 tvec = r.origin() - p0;
    float u = glm::dot(pvec, tvec) * invDet;
    if (u < 0.0f || u > 1.0f ) return false;

    Vec3 qvec = glm::cross(tvec, e1);
    float v = glm::dot(qvec, r.direction()) * invDet;
    if (v < 0.0f || u + v > 1.0f) return false;
    
    float t = glm::dot(qvec, e2) * invDet;
    if (!rayT.contains(t)) return false;

    Point3 intersection = r.at(t);
   
    rec.t = t;
    rec.p = intersection;
    rec.material = mat;

    // Interpolate normal values from vertices
    Vec3 normal = v0.normal*(1-u-v) + v1.normal*u + v2.normal*v; 
    rec.setFaceNormal(r, normal);
    
    // Interpolate texture coordinates (u and v)
    // (different meaning from barycentric coordinates)
    rec.u = v0.texCoords.x*(1-u-v) + v1.texCoords.x*u + v2.texCoords.x*v;
    rec.v = v0.texCoords.y*(1-u-v) + v1.texCoords.y*u + v2.texCoords.y*v;

    return true;
}

void Triangle::setBoundingBox() {
    // Compute the triangle's bounding box
    Point3 p0 = v0.position;
    Point3 p1 = v1.position;
    Point3 p2 = v2.position;

    float minX = fmin(p0.x, fmin(p1.x, p2.x));
    float minY = fmin(p0.y, fmin(p1.y, p2.y));
    float minZ = fmin(p0.z, fmin(p1.z, p2.z));

    float maxX = fmax(p0.x, fmax(p1.x, p2.x));
    float maxY = fmax(p0.y, fmax(p1.y, p2.y));
    float maxZ = fmax(p0.z, fmax(p1.z, p2.z));

    bbox = Aabb(Point3(minX, minY, minZ), Point3(maxX, maxY, maxZ));
}