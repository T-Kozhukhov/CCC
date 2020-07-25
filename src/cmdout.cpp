// TODO: test functionality of this class
// TODO: remove all mentions of std::cout from the remainder of the code - It must go through this class!
// TODO: compile and fix errors as necessary

#include "cmdout.h"

#include <iostream>

bool cmdout::verbose; //defining static members here

cmdout::cmdout(){
    //ctor
    cmdout::verbose = false; // verbosity is set to false by default
}

cmdout::~cmdout(){
    //dtor
}

void cmdout::setVerbose(){
    cmdout::verbose = true;
} 

void cmdout::cmdWrite(bool essential, std::string msg){
    if(essential || cmdout::verbose){
        std::cout << msg << '\n';
    }
}