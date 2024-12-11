#ifndef DEBUGGER_H
#define DEBUGGER_H

// header file for raytrace debugger

// include
#include "utility.h"

// debugger
class debugger
{
public:
    // get instance of debugger to be able to call from any class
    static debugger &getInstance()
    {
        static debugger instance;

        return instance;
    }

    // log to debugger file
    void logToFile(const std::string &message, const std::string &filename = "debug.log")
    {
        std::ofstream file(filename, std::ios::app);

        if (file.is_open())
        {
            file << "[LOG]: " << message << std::endl;
            file.close();
        }

        else
        {
            std::cerr << "[ERROR]: Could not open file: " << filename << std::endl;
        }
    }
};

#endif