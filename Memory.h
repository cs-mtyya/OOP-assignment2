#ifndef MEMORY_H
#define MEMORY_H
#include "IMemoryMappedDevice.h"
#include <cstdint>
#include <array>
#include <map>
#include <vector>



class Memory {
    private:    
    std::array<uint8_t, 256> memory;    
    std::map<uint8_t, IMemoryMappedDevice*> devices;


    public:   
    Memory(); 
    uint8_t read(uint8_t address);    
    void write(uint8_t address, uint8_t value);    
    void mapDevice(uint8_t address, IMemoryMappedDevice* device);    
    void clear();

    void loadProgram(uint8_t startAddress, const std::vector<uint8_t>& bytes);
};

#endif