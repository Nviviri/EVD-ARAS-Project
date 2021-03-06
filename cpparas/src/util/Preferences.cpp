#include "util/Preferences.hpp"
#include "debug/Debug.hpp"
#include <fstream>
#include <iostream>
#include <string>

namespace cpparas {

Preferences Preferences::fromFile(const std::string& filePath)
{
    std::ifstream inFile(filePath);
    Preferences result;

    if (!inFile.good())
        return result;

    std::string attrName;
    std::string value;
    char _c;
    inFile >> attrName;
    // Read single character between attribute name and value.
    inFile.get(_c);
    while (std::getline(inFile, value)) {
        if (attrName == "lastImageFile") {
            result.lastImageFile = value;
        } else if (attrName == "lastSequenceFile") {
            result.lastSequenceFile = value;
        } else {
            Debug::println(std::string("Unknown attribute ") + attrName);
        }
        inFile >> attrName;
        inFile.get(_c);
    }
    inFile.close();
    return result;
}

void Preferences::saveToFile(const std::string& filePath)
{
    Preferences oldPreferences = Preferences::fromFile(filePath);

    std::ofstream outFile(filePath);
    outFile << "lastImageFile " << (lastImageFile == "" ? oldPreferences.lastImageFile : lastImageFile) << std::endl;
    outFile << "lastSequenceFile " << (lastSequenceFile == "" ? oldPreferences.lastSequenceFile : lastSequenceFile) << std::endl;
    outFile.close();
}

} // namespace cpparas
