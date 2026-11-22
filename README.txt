VOLE MACHINE GUI - INSTALLATION AND RUNNING
===========================================

The GUI is a Qt 6 desktop application. It is built from the source in the gui
folder and uses the simulator source files in the repository root.

OPTION 1: RUN THE INCLUDED BUILD (ON THE BUILD MACHINE)
------------------------------------------------------
A prebuilt executable is included at:

    gui\build\release\VoleMachineQt.exe

The executable needs the matching Qt 6 and MinGW runtime libraries. On the
machine used to build this project, open an MSYS2 UCRT64 terminal and run it
from the repository root with:

    ./gui/build/release/VoleMachineQt.exe

If Windows reports that a Qt DLL is missing, use the build instructions below
or install/deploy the matching Qt runtime. Copying the EXE alone to another
computer is not sufficient.

OPTION 2: INSTALL THE BUILD TOOLS AND BUILD THE GUI
---------------------------------------------------
1. Install MSYS2 from https://www.msys2.org/.
2. Open the MSYS2 UCRT64 terminal (not the MSYS or MinGW 32-bit terminal).
3. Update MSYS2 when prompted, then install the UCRT64 C++ toolchain, make,
   and Qt 6 base/Widgets packages:

       pacman -Syu
       pacman -S --needed mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-qt6-base

   If the update asks you to close the terminal, reopen UCRT64 and run the
   package installation command again.
4. In the UCRT64 terminal, change to the repository's gui\build directory.
   For example, if the repository is in C:\Users\YourName\Documents\GitHub\OOP-assignment2:

       cd /c/Users/YourName/Documents/GitHub/OOP-assignment2/gui/build

5. Generate the build files and compile the Release GUI:

       qmake ../VoleMachineQt.pro
       mingw32-make

6. Run the application from the same UCRT64 terminal:

       ./release/VoleMachineQt.exe

The build output is gui\build\release\VoleMachineQt.exe. Use the same Qt
installation and MinGW toolchain for both qmake and compilation; mixing
compilers or Qt kits can cause build or runtime errors.

ALTERNATIVE: QT CREATOR
-----------------------
Install Qt 6 with a MinGW kit and the Qt Widgets component using the Qt online
installer. Open gui\VoleMachineQt.pro in Qt Creator, select the matching Qt 6
MinGW kit, then choose Build > Build Project and Run. The selected kit must
include the compiler corresponding to that Qt installation.

BASIC USE
---------
- Enter 4-digit hexadecimal instructions in the Code Editor, or use Browse to
  open a program text file such as program7.txt.
- Set the starting memory address in hexadecimal (00 by default), then load
  the editor contents into memory.
- Use Run to execute, Step to execute one instruction, and Reset to reset the
  machine. The GUI also provides register and memory editing controls.

TROUBLESHOOTING
---------------
- "qmake: command not found": ensure the MSYS2 UCRT64 Qt 6 package is installed
  and that the UCRT64 terminal is being used.
- "g++" or "mingw32-make" not found: install the UCRT64 toolchain and make
  packages listed above, then reopen the UCRT64 terminal.
- Missing Qt DLLs when launching outside UCRT64: run the application from the
  UCRT64 terminal, or deploy the required Qt runtime files with the matching
  Qt deployment tools before distributing the application.
