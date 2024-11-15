#include "utilities.hpp"

using std::string;
using std::ifstream, std::ofstream;

using namespace comUtils;

void updateMVP(const Shader &program, MVP mvp){
    program.setUniformMatrix("model", mvp.model);
    program.setUniformMatrix("view", mvp.view);
    program.setUniformMatrix("projection", mvp.projection);
}

namespace seOutput {

void writeOutput(Camera camera, float focusPlaneDistance){
    using namespace details;
    
    std::ifstream inFile(INPUT_FILE);
    if (inFile.fail()) fatalError(string("Error: failed opening file ")+INPUT_FILE);

    string outFileName = "scene_explorer_output_temp";
    std::ofstream outFile(outFileName);
    if (outFile.fail()) fatalError("Error: failed opening file " + outFileName);
    // line of definition of `lookFrom` vector
    unsigned int lookFromLine = 19; 

    copyLines(inFile, outFile, lookFromLine);
    // write `lookFrom` vector
    writeVector(inFile, outFile, camera.getPosition());

    copyLines(inFile, outFile, 2);
    // write `lookAt` vector
    writeVector(inFile, outFile, camera.getPosition()+camera.getFront());

    copyLines(inFile, outFile, 2);
    // write `up` vector
    writeVector(inFile, outFile, camera.getUp());

    copyLines(inFile, outFile, 5);
    // write focus plane distance
    outFile << "- Focus Distance : " << focusPlaneDistance << "\n";    
    input::details::skipLines(inFile, 1);

    // Copy the rest until you reach EOF
    string line;
    while(std::getline(inFile, line)) {
        outFile << line << "\n";
    }
    inFile.close();
    outFile.close();
    // Overwrite input file
    if (std::rename(outFileName.c_str(), INPUT_FILE) != 0){
        fatalError("Error renaming file "+outFileName+" into "+INPUT_FILE);
    }
    std::cout << "Camera data has been written to " << INPUT_FILE << "\n";
}

void details::copyLines(ifstream& inFile, ofstream& outFile, unsigned int n) {
    string line;
    for (unsigned int i = 0; i < n; i++) {
        if (!std::getline(inFile, line)) {
            fatalError(string("Error: unexpectedly reached EOF while reading from ")+INPUT_FILE);
        }
        outFile << line << "\n";
    }
}

void details::writeVector(ifstream& inFile, ofstream& outFile, glm::vec3 vec){
    outFile << "  - x : " << vec.x << "\n";
    outFile << "  - y : " << vec.y << "\n";
    outFile << "  - z : " << vec.z << "\n";
    input::details::skipLines(inFile, 3);
}

} // namespace seOutput