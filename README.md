# CCC
Custom Crowds in C++. Software designed to simulate dense crowds of a single species using the Vicsek model.

##System Prerequisites
Primarily designed for Ubuntu Linux however works on Windows via WSL and MacOS. Only tested using the g++ compiler. 

To compile and use the program, the following must be installed:
- CMake. Set to require version 3.1 or above but easily changed by an experienced user.
- GNU Scientific Library (I used v2.4)
- Visualisation ToolKit (VTK) **v6 or above is required** (I used v6.3)

GSL and VTK can be installed on Ubuntu using the following top-level packages:
```
libgsl-dev
libvtk6-dev
```
Output .vtp visualisation files can be read using the ParaView software.

##Running the program
To test run the program I suggest copying the contents of the TESTCASE folder to the same directory as the program and then running the RunMe bash script. An Experiment sub-folder will be created, with .vtp files stored within Experiment/VTP/
