#ifndef SIMULATOR_UI_H
#define SIMULATOR_UI_H
#include <iostream>
#include "Vole_machine.h" 


class SimulatorUI {
private:
    VoleMachine machine; 

public:
    void displayMenu();
    void getUserInput();
    void showMachineState();
    void startExecution();
};

#endif