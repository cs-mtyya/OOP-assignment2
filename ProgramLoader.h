#ifndef PROGRAMLOADER_H
#define PROGRAMLOADER_H
#include <string>
#include <vector>
#include <cstdint>
class ProgramLoader {
public:
    std::vector<uint8_t> loadFromFile(std::string fileName);
    bool validateFormat(std::string line);
    bool validateStartAddress(uint8_t startAddress, size_t programSize);
};

#endif