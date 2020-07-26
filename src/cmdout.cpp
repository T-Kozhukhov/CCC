// TODO: test functionality of this class

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