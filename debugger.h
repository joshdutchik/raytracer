#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

class debugger
{
public:
    static debugger &getInstance()
    {
        static debugger instance;
        return instance;
    }

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