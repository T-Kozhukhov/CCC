#ifndef CMDOUT_H
#define CMDOUT_H

#include <string>

/*
    A static singleton class designed to handle output to the console (hence, cmdout). Main use is for dealing with
        verbose output from the software. 
*/

class cmdout
{
    public:
        cmdout();
        ~cmdout();

        static void setVerbose(); //sets the flag for the cmd output to be verbose

        static void cmdWrite(bool essential, std::string msg); //write message if essential or verbose output enabled
    
    private:
        static bool verbose;
}

#endif // CMDOUT_H
