#include <fstream>
#include <iostream>
#include "Memory.h"

int main()
{
    std::string fileName = "input.txt";
    std::ifstream inputFile (fileName.c_str());

    Memory myMemory(inputFile);

    MemoryDump& memoryHistory = MemoryDump::getInstance();

    std::cout<<"Solution of part I: "<<myMemory.getNumberOfFirstRepeated (memoryHistory)<<std::endl;

    memoryHistory.clearMemoryHistory();

    std::cout<<"Solution of part II: "<<myMemory.getNumberOfFirstRepeated (memoryHistory)<<std::endl;

    inputFile.close();

    return 0;
}
