#include "ProgramLoader.h"
#include "Instruction.h"
#include <fstream>
#include <cctype>
#include <iostream>


std::vector<uint8_t> ProgramLoader::loadFromFile(std::string fileName) {
    std::vector<uint8_t> program;
    std::ifstream file(fileName);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: File missing or unreadable.\n";
        return {}; 
    }

    int lineNumber = 0;
    while (std::getline(file, line)) {
        lineNumber++;
        std::string instruction = "";

        for (int i = 0; i < line.length(); i++) {
            if (line[i] == ';') break;
            if (!std::isspace(line[i])) {
                instruction += line[i];
            }
        }

        if (instruction.empty()) continue;

        if (instruction.length() != 4 || !validateFormat(instruction)) {
            std::cerr << "Error: Invalid instruction format on line " << lineNumber << ".\n";
            return {};
        }

        Instruction inst;
        if (!Instruction::parseHexWord(instruction, inst) || !inst.isValid()) {
            std::cerr << "Error: Invalid instruction on line " << lineNumber << ".\n";
            return {};
        }

        int byte1 = std::stoi(instruction.substr(0, 2), nullptr, 16);
        int byte2 = std::stoi(instruction.substr(2, 2), nullptr, 16);

        program.push_back(byte1);
        program.push_back(byte2);
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
    if (startAddress % 2 != 0) {std::cerr << "Error: Start address (0x" << std::hex << (int)startAddress 
                  << std::dec << ") must be even.\n";
        return false;
    }
    
    if (startAddress + programSize > 256) {
        std::cerr << "Error: Program size exceeds available memory. Try a lower start address.\n";
        return false;
    } 
    return true;
}