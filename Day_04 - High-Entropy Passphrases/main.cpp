#include "PassPhrasesChecker.h"
#include <iostream>
#include <fstream>
#include <cstdlib>

int main()
{
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open())
    {
        std::cout<<"Could not open input file"<<std::endl;
        exit(-1);
    }

    PassPhrasesChecker checkInput;
    checkInput.checkFile(inputFile);

    std::cout<<"Number of passphrases for part 1: "<<checkInput.getCounterForPartOne()<<std::endl;

    std::cout<<"Number of passphrases for part 2: "<<checkInput.getCounterForPartTwo()<<std::endl;

    inputFile.close();

    return 0;
}
