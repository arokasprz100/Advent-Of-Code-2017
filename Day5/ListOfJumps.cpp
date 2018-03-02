#include "ListOfJumps.h"
#include <string>
#include <cstdlib>
#include <iostream>

ListOfJumps::ListOfJumps(std::ifstream& fileToParse)
{
    m_jumpCounter = 0;
    m_jumperPosition = 0;
    std::string lineFromFile;
    while (getline(fileToParse, lineFromFile ))
        m_jumps.push_back(atoi(lineFromFile.c_str()));
}

void ListOfJumps::printVector() const
{
    for (unsigned i=0; i<m_jumps.size(); ++i)
        std::cout<<m_jumps.at(i)<<" ";
    std::cout<<std::endl;
}

void ListOfJumps::traverseVector(bool isPartTwo)
{
    int sizeOfVector = m_jumps.size();
    std::vector<int> copy = m_jumps;
    while (true)
    {
        if (m_jumperPosition<0 || m_jumperPosition>=sizeOfVector)
            break;

        int currentPosition = m_jumperPosition;

        m_jumperPosition+=copy.at(m_jumperPosition);

        if (isPartTwo)
        {
            if (copy.at(currentPosition)>=3)
                copy.at(currentPosition)--;
            else
                copy.at(currentPosition)++;

        }
        else
            copy.at(currentPosition)++;

        m_jumpCounter++;
    }
}

int ListOfJumps::getJumpCounter() const
{
    return m_jumpCounter;
}

void ListOfJumps::resetJumper()
{
    m_jumpCounter = 0;
    m_jumperPosition = 0;
}
