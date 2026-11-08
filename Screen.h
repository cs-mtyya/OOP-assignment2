#ifndef SCREEN_H
#define SCREEN_H
#include "IMemoryMappedDevice.h"
#include <cstdint>
#include <string>


class Screen : public IMemoryMappedDevice {
    
    private:    
    std::string outputBuffer;

    public:    
    void writeToDevice(uint8_t value) override;    
    uint8_t readFromDevice() override;    
    std::string getOutput() const;    
    void clear();

};

#endif