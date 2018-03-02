#include "Memory.h"
#include <iostream>

Memory::Memory (std::ifstream& inputFile)
{
    int value;
    while (true)
    {
        inputFile >> value;
        if (inputFile.eof())
            break;
        m_listOfBanks.push_back(value);
    }
}


void Memory::saveState (MemoryDump& savePlace)
{
    savePlace.addState(m_listOfBanks);
}

unsigned Memory::findBankWithMostBlocks()
{
    int maxIndex = 0;
    for (unsigned i=1; i<m_listOfBanks.size(); ++i)
    {
        if (m_listOfBanks.at(i)>m_listOfBanks.at(maxIndex))
            maxIndex = i;
    }
    return maxIndex;
}

void Memory::redistribution()
{
    int maxIndex = findBankWithMostBlocks();
    int numberOfBlocksToRedistribute = m_listOfBanks.at(maxIndex);
    m_listOfBanks.at(maxIndex) = 0;
    int currentIndex = (maxIndex+1)%16;
    for (int i=0; i<numberOfBlocksToRedistribute; ++i)
    {
        (m_listOfBanks.at(currentIndex))++;
        currentIndex = (currentIndex+1)%16;
    }
}

unsigned Memory::getNumberOfFirstRepeated  (MemoryDump& allStates)
{
    do
    {
        saveState(allStates);
        redistribution();
    }while (!checkIfOccured(allStates));

    return allStates.m_numberOfStates;
}

bool Memory::checkIfOccured (MemoryDump& allStates)
{
    if (allStates.m_allStates.find(m_listOfBanks)!= allStates.m_allStates.end())
        return true;
    return false;
}
