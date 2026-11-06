#ifndef SCREEN_H
#define SCREEN_H
#include "IMemoryMappedDevice.h"
#include <cstdint>
#include <string>
using namespace std;

class Screen : public IMemoryMappedDevice {
    
    private:    
    string outputBuffer;

    public:    
    void writeToDevice(uint8_t value) override;    
    uint8_t readFromDevice() override;    
    string getOutput();    
    void clear();

};

#endif