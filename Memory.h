#ifndef MEMORY_H
#define MEMORY_H
#include "IMemoryMappedDevice.h"
#include <cstdint>
#include <array>
#include <map>
using namespace std;


class Memory {
    private:    
    array<uint8_t, 256> cells;    
    map<uint8_t, IMemoryMappedDevice*> devices;


    public:    
    uint8_t read(uint8_t address);    
    void write(uint8_t address, uint8_t value);    
    void mapDevice(uint8_t address, IMemoryMappedDevice* device);    
    void clear();

};





#endif