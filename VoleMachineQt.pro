QT += widgets
CONFIG += c++17
TEMPLATE = app
TARGET = VoleMachineQt

SOURCES += \
    qt_main.cpp \
    MainWindow.cpp \
    CPU.cpp \
    Instruction.cpp \
    Memory.cpp \
    ProgramLoader.cpp \
    RegisterFile.cpp \
    Screen.cpp \
    Volemachine.cpp

HEADERS += \
    MainWindow.h \
    CPU.h \
    IMemoryMappedDevice.h \
    Instruction.h \
    Memory.h \
    ProgramLoader.h \
    RegisterFile.h \
    Screen.h \
    Vole_machine.h