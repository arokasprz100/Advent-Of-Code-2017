#include "Day1Functions.h"
#include <iostream>

std::vector<int>& copyFileToVector (std::vector<int> & destination, std::fstream& inputFile)
{
    while (true)
    {
        char added = inputFile.get();
        if (inputFile.eof() || added=='\n')
            break;
        destination.push_back(added - '0');
    }

    return destination;
}

void printVector (std::vector<int>& printedVector)
{
    std::cout<<"Numbers stored in vector: "<<std::endl;
    for (unsigned int i=0; i<printedVector.size(); ++i)
        std::cout<<printedVector.at(i);

    std::cout<<std::endl;
}

unsigned int sumInPartOne (std::vector<int>& vectorOfNumbers)
{
    unsigned int sumOfElements = 0;

    for (unsigned int i=0; i<vectorOfNumbers.size()-1; ++i)
    {
        if (vectorOfNumbers.at(i)==vectorOfNumbers.at(i+1))
            sumOfElements+=vectorOfNumbers.at(i);
    }

    if (vectorOfNumbers.at(vectorOfNumbers.size()-1)== vectorOfNumbers.at(0))
        sumOfElements+=vectorOfNumbers.at(vectorOfNumbers.size()-1);

    return sumOfElements;
}

unsigned int sumInPartTwo (std::vector<int>& vectorOfNumbers)
{
    unsigned int sumOfElements = 0;
    unsigned int halfOfSize = vectorOfNumbers.size()/2;

    for (unsigned int i=0; i<vectorOfNumbers.size(); ++i)
    {
        if (vectorOfNumbers.at(i)==vectorOfNumbers.at((i+halfOfSize)%(halfOfSize*2)))
            sumOfElements+=vectorOfNumbers.at(i);
    }



    return sumOfElements;

}
