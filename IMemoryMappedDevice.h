#ifndef IMEMORYMAPPEDDEVICE_H
#define IMEMORYMAPPEDDEVICE_H
#include <cstdint>


class IMemoryMappedDevice{
    public:
    virtual ~IMemoryMappedDevice() = default;
    virtual void writeToDevice(uint8_t value) = 0;
    virtual uint8_t readFromDevice() = 0;

};

#endif