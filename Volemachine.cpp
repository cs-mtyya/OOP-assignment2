#include "VoleMachine.h" 
#include <iostream>


VoleMachine::VoleMachine() {}

bool VoleMachine::loadProgram(const std::string& fileName, uint8_t startAddress) {
    return loader.load(fileName, memory, startAddress);
}

void VoleMachine::run() {
    while (cpu.isRunning()) {
        step();
    }
}

void VoleMachine::step() {
    cpu.executeStep(memory, registers, screen);
}

void VoleMachine::reset() {
    cpu.reset();
    memory.reset();
    registers.reset();
    screen.clear();
}

void VoleMachine::showStatus() {
    cpu.display();
    registers.display();
    memory.display();
    screen.display();
}

void VoleMachine::editMemory(uint8_t address, uint8_t value) {
    memory.setCell(address, value); 
}

void VoleMachine::editRegister(uint8_t id, uint8_t value) {
    registers.setRegister(id, value); 
}