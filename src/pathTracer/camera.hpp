#pragma once

#include "myPT.hpp"
#include "hittable.hpp"
#include "ray.hpp"
#include "material.hpp"
#include "utilities.hpp"

#include <filesystem>
#include <mutex>
#include <random>
#include <algorithm>

class Camera {
    public:
        // `up` vector doesn't have to be orthogonal to the other two
        Camera (float aspectRatio = 1.0f, int imageWidth = 100, float vfov = 90, 
                const Point3& lookFrom = Point3(0.0f,0.0f,0.0f),
                const Point3& lookAt = Point3(0.0f,0.0f,-1.0f),
                const Vec3& up = Vec3(0.0f,1.0f,0.0f));

        // Render output image
        void render(const Hittable& world);
        
        // Setters
        void setImageName(std::string name) {
            imageName = name;
            subImagesDir = std::string(OUTPUT_DIR) + "/" + imageName;
        }
        void setSamplesPerPixel(int n){samplesPerPixel = n;}
        void setDefocusAngle(float angle){defocusAngle = angle;}
        void setFocusDist(float d){focusDist = d;}
        void setMaxDepth(int n){maxDepth = n;}
        void setBackground(Color color){background = color;}
    
    private:    
        // Width over height
        float aspectRatio;
        // Rendered image width
        int imageWidth; 
        int imageHeight;
        // Vertical field of view
        float vfov;     

        // Camera position and orientation
        Point3 lookFrom;
        Point3 cameraCenter = lookFrom;
        Point3 lookAt;
        Vec3 up;

        // Output image name
        std::string imageName;
        
        // Number of random samples per pixel
        int samplesPerPixel;  

        // Variation angle of rays through each pixel
        float defocusAngle;
        // Distance from camera center to plane of perfect focus
        float focusDist;
        // Defocus disk horizontal radius
        Vec3 defocusDiskU; 
        // Defocus disk vertical radius
        Vec3 defocusDiskV; 
        
        // Maximum number of ray bounces
        int maxDepth;

        // Scene background color
        Color background; 

        // Location of pixel (0,0)
        Point3 pixel00;  
        // Offset to pixel to the right
        Vec3 pixelDeltaU;  
        // Offset to pixel below
        Vec3 pixelDeltaV;  
        
        Color rayColor(const Ray& r, int depth, const Hittable& world) const;
        void initialize();
        
        // Constructs a ray originating from the camera and directed at a randomly
        // sampled point around the pixel location (i,j)
        Ray getRay(int i, int j) const;
        
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square
        Vec3 sampleUnitSquare() const;
        // Returns a random point in the camera defocus disk
        Point3 defocusDiskSample() const;

        struct SubImage {
            Interval rows;
            std::string path;
        };
        // number of sub-images
        int numSubImages;
        // directory where sub-images are kept
        std::string subImagesDir; 

        struct SubImageList {
            // elements of list
            std::vector<SubImage> data;
            // this mutex controls the access to `data`
            std::mutex mtx; 
        };

        // Task for concurrent threads:
        // Renders a sub-image from the `notRendered` list,
        // Ends when there are no longer sub-images that need rendering.
        // If `first` is set to true (first thread), the thread also logs
        // information about the program's progress
        void renderTask(const Hittable& world, SubImageList& notRendered,
                        SubImageList& rendered, bool first) const;
        
        // Set up information for sub-images that need to be rendered
        void setUpSubImages(SubImageList& notRendered);
        // Put together final image
        void putTogetherImage(SubImageList& rendered);
};