#pragma once

#include <fstream>
#include <sstream>

namespace comUtils {
namespace input {
namespace details {

template <typename T>
T readParameter(std::ifstream& inputStream) {
    std::string line;
    getline(inputStream, line);
    std::istringstream iss(line);
    std::string s;

    int count = 0; 
    do {
        // loop limit
        count++;
        if (count > 300) {
            fatalError(std::string("Error: parameter from input file ") + INPUT_FILE +
                       " not found. Input file could be misconfigured.");
        }
        // Skip the strings before ":" (and skip that too)
        iss >> s;
    } while (s != ":");
    T parameter;
    iss >> parameter;
    return parameter;
}

template <typename T>
T readParameterAt(const std::string& inputFileName, unsigned int line) {
    std::ifstream inputFile;
    inputFile.open(inputFileName);
    if (inputFile.fail()) fatalError("Error: failed opening input file");

    details::skipLines(inputFile, line-1);
    return details::readParameter<T>(inputFile);
}

}
}
}