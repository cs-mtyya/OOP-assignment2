#ifndef VOLE_MACHINE_H
#define VOLE_MACHINE_H

#include <string>
#include <cstdint>
#include "CPU.h"
#include "Memory.h"
#include "RegisterFile.h"
#include "Screen.h"
#include "ProgramLoader.h"

class VoleMachine {
private:
    
    CPU cpu;
    Memory memory;
    RegisterFile registers;
    Screen screen;
    ProgramLoader loader;

public:
    
    VoleMachine();

    
    bool loadProgram(const std::string& fileName, uint8_t startAddress);
    void run();
    void step();
    void reset();
    void showStatus();
    void editMemory(uint8_t address, uint8_t value);
    void editRegister(uint8_t id, uint8_t value);
};

#endif 