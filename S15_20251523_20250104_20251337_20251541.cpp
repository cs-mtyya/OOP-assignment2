#include <iostream>
#include <string>
#include "Memory.h"
#include "RegisterFile.h"
#include "Screen.h"
#include "CPU.h"
#include "ProgramLoader.h"
#include "SimulatorUI.h"
using namespace std;


int main() {
    SimulatorUI ui;
    ui.getUserInput();
    return 0;
}