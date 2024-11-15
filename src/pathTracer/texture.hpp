#pragma once

#include "myPT.hpp"
#include "image.hpp"
#include "interval.hpp"

class Texture {
    public:
        virtual ~Texture() = default;
        virtual Color value(float u, float v, const Point3& p) const = 0;
};

class SolidColor : public Texture {
    public:
        SolidColor(const Color& albedo) : albedo(albedo) {}

        SolidColor(float r, float g, float b)
            : SolidColor(Color(r, g, b)) {}

        Color value(float u, float v, const Point3& p) const override {
            return albedo;
        }

    private:
        Color albedo;
};

using std::shared_ptr;
using std::make_shared;

class CheckerTexture : public Texture {
    public:
        CheckerTexture(float scale, shared_ptr<Texture> even, shared_ptr<Texture> odd)
            : scale(scale), even(even), odd(odd) {}

        CheckerTexture(float scale, const Color& c1, const Color& c2)
            : CheckerTexture(scale, make_shared<SolidColor>(c1), make_shared<SolidColor>(c2)) {}

        Color value(float u, float v, const Point3& p) const override {
            float invScale = 1.0f / scale;
            int xInteger = int(std::floor(invScale * p.x));
            int yInteger = int(std::floor(invScale * p.y));
            int zInteger = int(std::floor(invScale * p.z));

            bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

            return isEven ? even->value(u, v, p) : odd->value(u, v, p);
        }

    private:
        float scale;
        shared_ptr<Texture> even;
        shared_ptr<Texture> odd;
};

class ImageTexture : public Texture {
    public:
        ImageTexture(std::shared_ptr<Image> image) : image(image) {}

        Color value(float u, float v, const Point3& p) const override {
            // If we have no texture data, then return solid cyan as a debugging aid
            if (image->height() <= 0) return Color(0, 1, 1);

            // Clamp input texture coordinates to [0,1]
            u = Interval(0, 1).clamp(u);
            v = Interval(0, 1).clamp(v);

            #ifdef FLIP_Y_AXIS_TEXTURE
                // Flip v to image coordinates
                v = 1.0 - v;  
            #endif

            int i = int(u * image->width());
            int j = int(v * image->height());
            auto pixel = image->pixelData(i, j);

            auto colorScale = 1.0 / 255.0;
            return Color(colorScale * pixel[0], colorScale * pixel[1],
                         colorScale * pixel[2]);
        }

    private:
        std::shared_ptr<Image> image;
};