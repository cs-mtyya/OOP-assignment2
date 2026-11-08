#include "ProgramLoader.h"
#include <fstream>
#include <cctype>
std::vector<uint8_t> ProgramLoader::loadFromFile(std::string fileName) {
    std::vector<uint8_t> program;
    std::ifstream file(fileName);
    std::string line;
    while (std::getline(file, line)) {
        std::string instruction = "";

        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ';') break;
            if (!std::isspace(line[i])) {
                instruction += line[i];
            }
        }

        if (instruction.length() == 4 && validateFormat(instruction)) {
            int byte1 = std::stoi(instruction.substr(0, 2), nullptr, 16);
            int byte2 = std::stoi(instruction.substr(2, 2), nullptr, 16);

            program.push_back(byte1);
            program.push_back(byte2);
        }
    }
    return program;
}

bool ProgramLoader::validateFormat(std::string line) {
    for (int i = 0; i < line.length(); i++) {
        if (!std::isxdigit(line[i])) return false;
    }
    return true;
}

bool ProgramLoader::validateStartAddress(uint8_t startAddress, size_t programSize) {
    if (startAddress % 2 != 0) return false;
    if (startAddress + programSize > 256) return false;
    return true;
}