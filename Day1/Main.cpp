#include <iostream>
#include "Day1Functions.h"

int main()
{
    std::fstream inputFile;
    inputFile.open("input.txt", std::ios_base::in);
    if (!inputFile.is_open())
    {
        std::cout<<"Could not open a file."<<std::endl;
        return -1;
    }
    std::vector <int> vectorOfNumbers;

    vectorOfNumbers = copyFileToVector(vectorOfNumbers, inputFile);

    inputFile.close();

    printVector(vectorOfNumbers);

    std::cout<<"Sum in part one is equal: "<<sumInPartOne(vectorOfNumbers)<<std::endl;

    std::cout<<"Sum in part two is equal: "<<sumInPartTwo(vectorOfNumbers)<<std::endl;


    return 0;
}
