#include "Spreadsheet.h"

#include <iostream>
#include <fstream>

int main()
{
    std::fstream inputFile("input.txt", std::ios_base::in);
    if (!inputFile.is_open())
    {
        std::cerr<<"Could not open input file"<<std::endl;
        return -1;
    }

    Spreadsheet parsed;
    parsed.parseFile(inputFile);

    std::cout<<"Checksum for part 1 equals: "<<parsed.getChecksumFromPartOne()<<std::endl;
    std::cout<<"Checksum for part 2 equals: "<<parsed.getChecksumFromPartTwo()<<std::endl;

    return 0;
}
