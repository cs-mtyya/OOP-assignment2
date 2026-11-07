#include "Memory.h"

Memory::Memory() {
    clear();
}

void Memory::clear() {
    memory.fill(0);
}

void Memory::mapDevice(uint8_t address, IMemoryMappedDevice* device) {
    devices[address] = device;
}

uint8_t Memory::read(uint8_t address) {
    auto it = devices.find(address);
    if (it != devices.end()) {
        return it->second->readFromDevice();
    }
    return memory[address];
}
void Memory::write(uint8_t address, uint8_t value) {
    auto it = devices.find(address);
    if (it != devices.end()) {
        it->second->writeToDevice(value);
        return;
    }
    memory[address] = value;
}

void Memory::loadProgram(uint8_t startAddress, const std::vector<uint8_t>& bytes) {
    if (startAddress + bytes.size() > 256) {
        return; 
    }

for (size_t i = 0; i < bytes.size(); ++i) {
        memory[startAddress + i] = bytes[i];
    }
}