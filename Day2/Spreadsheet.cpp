#include "Spreadsheet.h"
#include <iostream>
#include <sstream>
#include <climits>
#include <cstdlib>

Spreadsheet::Spreadsheet()
{
    m_checksumPartOne = 0;
    m_checksumPartTwo = 0;
}

void Spreadsheet::parseFile(std::fstream& fileToParse)
{
    std::string line;
    while (getline(fileToParse, line))
        parseLine(line);
}

unsigned Spreadsheet::getChecksumFromPartOne() const
{
    return m_checksumPartOne;
}

unsigned Spreadsheet::getChecksumFromPartTwo() const
{
    return m_checksumPartTwo;
}

void Spreadsheet::parseLine (std::string line)
{
    std::vector<int> elements;
    std::string helper;
    std::stringstream ss(line);
    while (getline(ss, helper, '\t'))
    {
        elements.push_back(atoi(helper.c_str()));
    }

    printLine(elements);

    m_checksumPartOne+=findDiffMaxMin(elements);

    m_checksumPartTwo+=findEventlyDivisible(elements);

}

void Spreadsheet::printLine(const std::vector<int>& valuesFromLine)
{
    for (unsigned int i=0; i<valuesFromLine.size(); ++i)
    {
        std::cout<<valuesFromLine.at(i)<<" ";
    }
    std::cout<<std::endl;
}

unsigned Spreadsheet::findDiffMaxMin (std::vector<int>& numbers)
{
    int min = INT_MAX;
    int max = 0;
    for (unsigned int i=0; i<numbers.size(); ++i)
    {
        if (numbers.at(i)>max)
            max = numbers.at(i);
        if (numbers.at(i)<min)
            min = numbers.at(i);

    }

    return max - min;
}

unsigned Spreadsheet::findEventlyDivisible (std::vector<int>& numbers)
{
    for (unsigned int i = 0; i<numbers.size(); ++i)
    {
        for (unsigned int j = i+1; j<numbers.size(); j++)
        {
            if (numbers.at(i)%numbers.at(j)==0)
                return numbers.at(i)/numbers.at(j);
            else if (numbers.at(j)%numbers.at(i)==0)
                return numbers.at(j)/numbers.at(i);
        }
    }

    return 0;
}
