#include "camera.hpp"

using std::unique_lock;
using std::mutex;

Camera::Camera (float aspectRatio, int imageWidth, float vfov, 
                const Point3 &lookFrom, const Point3 &lookAt, const Vec3 &up)
                : aspectRatio(aspectRatio), imageWidth(imageWidth), vfov(vfov),
                lookFrom(lookFrom), lookAt(lookAt), up(up) {

    imageHeight = int(imageWidth / aspectRatio);
    imageHeight = (imageHeight < 1) ? 1 : imageHeight;
    numSubImages = ptInput::readNumSubImages(INPUT_FILE);
    // Set default values for other camera parameters
    setSamplesPerPixel(10);
    setDefocusAngle(0.0f);
    setFocusDist(1.0f);
    setMaxDepth(5);
    setBackground(Color(0.0f,0.0f,0.0f));    
    setImageName("helloPT");
}

void Camera::initialize() {
    // Determine viewport dimensions
    float theta = glm::radians(vfov); // from degrees to radians
    float h = tan(theta/2);
    float viewportHeight = 2 * h * focusDist;
    float viewportWidth = viewportHeight * (float(imageWidth)/imageHeight);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame
    Vec3 w = glm::normalize(lookFrom - lookAt);
    Vec3 u = glm::normalize(glm::cross(up, w));
    Vec3 v = glm::cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges
    Vec3 viewportU = viewportWidth * u;
    Vec3 viewportV = viewportHeight * -v;

    // Calculate the horizontal and vertical delta vectors from pixel to pixel
    pixelDeltaU = viewportU / float(imageWidth);
    pixelDeltaV = viewportV / float(imageHeight);

    // Calculate the location of the upper left pixel
    Point3 viewportUpperLeft = cameraCenter - (focusDist * w) - viewportU/2.0f - viewportV/2.0f;
    pixel00 = viewportUpperLeft + 0.5f * (pixelDeltaU + pixelDeltaV);

    // Calculate the camera defocus disk radius
    float defocusRadius = focusDist * tan(glm::radians(defocusAngle/2));
    defocusDiskU = u * defocusRadius;
    defocusDiskV = v * defocusRadius;
}

void Camera::render(const Hittable& world) {
    initialize();
    // Each thread renders a randomly chosen sub-image
    // that hasn't been rendered yet
    std::vector<std::thread> threads;
    // sub-images that have yet to be rendered
    SubImageList notRendered;
    // sub-images that have already been rendered by a thread
    SubImageList rendered;    
    setUpSubImages(notRendered);

    std::clog << "Rendering sub-images in " << subImagesDir << "\n";

    // Set up threads
    int nThreads = ptInput::readNumThreads(INPUT_FILE);
    bool first = true;
    for (int i = 0; i < nThreads; i++) {
        threads.push_back(
            std::thread(&Camera::renderTask,
                        this,
                        std::ref(world),
                        std::ref(notRendered),
                        std::ref(rendered),
                        first));
        first = false;
    }
    // Wait for the threads to finish
    for (int i = 0; i < nThreads; i++){
        threads[i].join();
    }
    putTogetherImage(rendered);
    std::cout << "Done!\n\n";
}

void Camera::renderTask(const Hittable& world, SubImageList& notRendered,
                        SubImageList& rendered, bool first) const {
    
    std::ofstream outFile; // output .ppm file
    SubImage subImage; // sub-image that's being currently rendered

    if (first) {std::clog << "0 sub-images out of " << numSubImages << " have been rendered\n";}

    while (true) {
        unique_lock<mutex> lock1{notRendered.mtx};  
        if (notRendered.data.empty()) break;      
        subImage = notRendered.data.back();
        notRendered.data.pop_back();            
        lock1.unlock();

        outFile.open(subImage.path);
        if (outFile.fail()) fatalError("Error: failed opening output file " + subImage.path);
        outFile << "P3\n" << imageWidth << ' ' << subImage.rows.max-subImage.rows.min + 1 << "\n255\n";

        int firstRow = subImage.rows.min;
        int lastRow = subImage.rows.max;
        for (int j = firstRow; j <= lastRow; j++) {
            if (first) {
                std::clog << "\rThread #1 is "
                          << int((float(j-firstRow)/(subImage.rows.size()+1))*100)
                          << "% done with current sub-image " 
                          << "(" << lastRow-j+1 << " rows left)    "
                          << std::flush;
            }
            for (int i = 0; i < imageWidth; i++) {
                Color pixelColor(0.0f,0.0f,0.0f);
                for (int sample = 0; sample < samplesPerPixel; sample++) {
                    Ray r = getRay(i, j);
                    pixelColor += rayColor(r, maxDepth, world); 
                }
                writeColor(outFile, pixelColor * (1.0f/samplesPerPixel));
            }
        }
        outFile.close();
        if (first) {std::clog << "\rThread #1 has rendered sub-image " << subImage.path << "\n";}

        unique_lock<mutex> lock2{rendered.mtx};
        rendered.data.push_back(subImage);
        if (first) { 
            std::clog << rendered.data.size() << " sub-images out of "
                      << numSubImages <<  " have been rendered\n";
        }
        lock2.unlock();
    }
}

Ray Camera::getRay(int i, int j) const{
    Vec3 offset = sampleUnitSquare(); 
    Point3 pixelSample = pixel00 + ((float(i)+offset.x)*pixelDeltaU) + ((float(j)+offset.y)*pixelDeltaV);

    Point3 rayOrigin = (defocusAngle <= 0) ? cameraCenter : defocusDiskSample();
    Vec3 rayDirection = pixelSample - rayOrigin;
    
    return Ray(rayOrigin, rayDirection);  
}

Color Camera::rayColor(const Ray& r, int depth, const Hittable& world) const {
    if (depth <= 0){
        // ray bounce limit exceeded
        return Color(0.0f, 0.0f, 0.0f);
    }
    HitRecord rec;
    // If the ray hits nothing, return the background color
    if (!world.hit(r, Interval(0.001, infinity), rec)){
        return background;
    }

    Ray scattered;
    Color attenuation;
    Color colorFromEmission = rec.material->emitted(rec.u, rec.v, rec.p);
    if (!rec.material->scatter(r, rec, attenuation, scattered)){
        return colorFromEmission;
    }
    Color colorFromScatter = attenuation * rayColor(scattered, depth-1, world);
    return colorFromEmission + colorFromScatter;
}

Vec3 Camera::sampleUnitSquare() const {
    return Vec3(randomFloat() - 0.5, randomFloat() - 0.5, 0);
}

Point3 Camera::defocusDiskSample() const {
    Vec3 v = randomInUnitDisk();
    return cameraCenter + (v.x * defocusDiskU) + (v.y * defocusDiskV);
}

void Camera::setUpSubImages(SubImageList& notRendered){
    if (!std::filesystem::create_directory(subImagesDir)) {
        fatalError("Error: failed creating directory " + subImagesDir);
    }
    int rowsPerSubImage = imageHeight/numSubImages;
    // first and last rows of current sub-image
    int firstRow = 0;
    int lastRow = rowsPerSubImage-1;
    for (int i = 0; i < numSubImages; i++) {
        SubImage subImage;
        subImage.rows = Interval(firstRow, lastRow);        
        subImage.path = subImagesDir + "/" + imageName + std::to_string(firstRow)
                        + "-" + std::to_string(lastRow) + ".ppm";
        notRendered.data.push_back(subImage);
        firstRow = lastRow+1;
        lastRow += rowsPerSubImage;
        
        if (i == numSubImages-2) {
            // The sub-image in the next iteration is going to be the last one,
            // so we add to it the rows that would be left out.
            int rowsLeft = imageHeight % numSubImages;
            lastRow += rowsLeft;
        }
    }
    // Shuffle the vector of sub-images
    std::random_device rd;
    std::mt19937 generator{rd()};
    std::shuffle(notRendered.data.begin(), notRendered.data.end(), generator);
}

void Camera::putTogetherImage(SubImageList& rendered) {
    std::ofstream finalImageFile; 
    std::string finalImagePath = std::string(OUTPUT_DIR) + "/" + imageName + ".ppm"; 
    finalImageFile.open(finalImagePath);
    if (finalImageFile.fail()) fatalError("Error: failed opening output file " + finalImagePath);
    
    std::clog << "Writing full image to " << finalImagePath << "\n";

    finalImageFile << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    // Copy each sub-image into the final image
    std::sort(std::begin(rendered.data), std::end(rendered.data),
                [](SubImage& a, SubImage& b){return a.rows.min < b.rows.min;});
    for (int i = 0; i < numSubImages; i++){
        std::ifstream subImageFile;
        std::string subImagePath = rendered.data[i].path;  
        subImageFile.open(subImagePath);
        
        std::string line; 
        // skip 3 lines
        for (int i = 0; i < 3; i++) {
            getline(subImageFile, line);
        }
        // copy contents
        while (getline(subImageFile, line)) {
            finalImageFile << line << "\n";
        }
        subImageFile.close();
        // delete sub-image file
        std::filesystem::remove(subImagePath);
    }  
    finalImageFile.close();
    
    // Remove sub-images directory (now empty)
    std::filesystem::remove(subImagesDir);
}