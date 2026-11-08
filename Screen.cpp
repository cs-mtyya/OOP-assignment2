#include "Screen.h"
#include <string>

void Screen::writeToDevice(uint8_t value) {
    outputBuffer += std::to_string(value) + " ";
}

uint8_t Screen::readFromDevice() {
    return 0;
}

std::string Screen::getOutput() const {
    return outputBuffer;
}

void Screen::clear() {
    outputBuffer.clear();
}