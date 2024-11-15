#include "material.hpp"

bool Lambertian::scatter(const Ray& in, const HitRecord& rec, Color& attenuation,
                         Ray& scattered) const {
    Vec3 scatterDirection = rec.normal + randomUnitVector();

    // Catch degenerate scatter direction (all vector components near zero)
    if (nearZero(scatterDirection)) scatterDirection = rec.normal;

    scattered = Ray(rec.p, scatterDirection);
    attenuation = tex->value(rec.u, rec.v, rec.p);
    return true;
}

bool Metal::scatter(const Ray& in, const HitRecord& rec, Color& attenuation,
                    Ray& scattered) const {
    Vec3 reflected = glm::reflect(in.direction(), rec.normal);
    reflected = glm::normalize(reflected) + (fuzz * randomUnitVector());
    scattered = Ray(rec.p, reflected);
    attenuation = albedo;
    return (glm::dot(scattered.direction(), rec.normal) > 0);
}

bool Dielectric::scatter(const Ray& in, const HitRecord& rec, Color& attenuation,
                         Ray& scattered) const {
    attenuation = Color(1.0f, 1.0f, 1.0f);
    float eta = rec.frontFace ? (1.0 / refractionIndex) : refractionIndex;

    Vec3 unitDirection = glm::normalize(in.direction());
    float cosTheta = fmin(glm::dot(-unitDirection, rec.normal), 1.0);
    float sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    bool cannotRefract = eta * sinTheta > 1.0;
    Vec3 direction;

    if (cannotRefract || reflectance(cosTheta, eta) > randomFloat()) {
        direction = glm::reflect(unitDirection, rec.normal);
    } else {
        direction = glm::refract(unitDirection, rec.normal, eta);
    }

    scattered = Ray(rec.p, direction);
    return true;
}

float Dielectric::reflectance(float cosine, float refractionIndex) {
    // Use Schlick's approximation for reflectance
    float r0 = (1.0 - refractionIndex) / (1.0 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}