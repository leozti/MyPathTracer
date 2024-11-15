#pragma once

#include "myPT.hpp"
#include "ray.hpp"
#include "hittable.hpp"
#include "texture.hpp"
#include "utilities.hpp"

class Material {
    public:
    virtual bool scatter(const Ray& in, const HitRecord& rec,
                        Color& attenuation, Ray& scattered)
                        const { return false; }
    // Emitted light (no light by default)
    virtual Color emitted(float u, float v, const Point3& p) const {
        return Color(0.0f,0.0f,0.0f);
    }
    virtual ~Material() = default;
};

// Lambertian surface
class Lambertian : public Material {
    public:
        Lambertian(const Color& albedo) : tex(std::make_shared<SolidColor>(albedo)) {}
        
        Lambertian(std::shared_ptr<Texture> tex) : tex(tex) {}
        
        bool scatter(const Ray& in, const HitRecord& rec, Color& attenuation,
                    Ray& scattered) const override;
    private:
        std::shared_ptr<Texture> tex;
};

// Metal surface
class Metal : public Material {
    public:
        Metal(const Color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
        bool scatter(const Ray& in, const HitRecord& rec, Color& attenuation,
                    Ray& scattered) const override;
    private:
        Color albedo;
        float fuzz;
};

// Dielectric surface
class Dielectric : public Material {
    public:
        Dielectric(float refractionIndex) : refractionIndex(refractionIndex) {}  
        bool scatter(const Ray& in, const HitRecord& rec, Color& attenuation,
                    Ray& scattered) const override;

    private:
        // Refractive index in vacuum or air, or the ratio of the material's
        // refractive index over the refractive index of the enclosing media
        float refractionIndex;
        static float reflectance(float cosine, float refractionIndex);
};

class DiffuseLight : public Material {
    public:        
        DiffuseLight(const Color& emit) : emit(emit) {}
        
        Color emitted(float u, float v, const Point3& p) const override {
            return emit;
        }
    private:
        Color emit;
};