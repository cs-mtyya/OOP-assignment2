#include "SimulatorUI.h"
#include <iostream>
#include <string>

void SimulatorUI::displayMenu() {
    std::cout<< "\n Vole Machine Sim\n";
    std::cout<< "1- Load Program\n";
    std::cout<< "2- Overwrite\n";
    std::cout<< "3- Run Program\n";
    std::cout<< "4- Exit\n";
    std::cout<< "Choose:";
}
void SimulatorUI::getUserInput() {
    bool programLoaded = false;

    while (true) {
        int choice;
        displayMenu();

        if (!(std::cin >> choice)) {
            return;
        }

        if (choice == 1) {
            std::string name;
            int address;
            std::cout << "Enter program name: ";
            std::cin >> name;
            std::cout << "Enter memory address (Hex, even number): ";
            std::cin >> std::hex >> address >> std::dec;

            if (machine.loadProgram(name, address)) {
                programLoaded = true;
                std::cout << "Program loaded successfully.\n";
            } 
            else {
                programLoaded = false;
                std::cout << "Error: Loading program failed.\n";
            }
        }  
        else if (choice == 2) {
            int target;
            std::cout << "Overwrite: 1-Register, 2-Memory cell: ";
            std::cin >> target;

            if (target == 1) {
                int id, value;
                std::cout << "Register number (hex, 0-F): ";
                std::cin >> std::hex >> id;
                std::cout << "New value (hex, 00-FF): ";
                std::cin >> value >> std::dec;

                if (id < 0 || id > 15 || value < 0 || value > 255) {
                    std::cout << "Error: register or value out of range.\n";
                } 
                else {
                    machine.editRegister(id, value);
                    std::cout << "Register updated.\n";
                }
            } 
            else if (target == 2) {
                int address, value;
                std::cout << "Memory address (hex, 00-FF): ";
                std::cin >> std::hex >> address;
                std::cout << "New value (hex, 00-FF): ";
                std::cin >> value >> std::dec;

                if (address < 0 || address > 255 || value < 0 || value > 255) {
                    std::cout << "Error: address or value out of range.\n";
                } else {
                    machine.editMemory(address, value);
                    std::cout << "Memory cell updated.\n";
                }
            } 
            else {
                std::cout << "Invalid choice.\n";
            }
        } 
        else if (choice == 3) {
            if (!programLoaded) {
                std::cout << "Error: Cannot run. No valid program loaded.\n";
            } else {
                startExecution();
            }
        } else if (choice == 4) {
            return;
        } else {
            std::cout << "Invalid choice.\n";
        }
    }
}

void SimulatorUI::showMachineState() {
    std::cout<< "\nCPU & Memory State\n";
    machine.showStatus();
}
void SimulatorUI::startExecution() {
    int mode;
    std::cout<< "Mode (1-Run All, 2-Step by Step): ";
    std::cin>> mode;
    if (mode == 1) {
        machine.run();
        showMachineState(); 
    } 
    else if (mode == 2) {
       machine.runStepMode();
        }
    }