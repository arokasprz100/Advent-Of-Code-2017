#ifndef MEMORY_H
#define MEMORY_H

#include <vector>
#include <fstream>
#include "MemoryDump.h"


class Memory
{
public:
    Memory (std::ifstream& inputFile);
    unsigned getNumberOfFirstRepeated (MemoryDump& allStates);

private:
    std::vector<int> m_listOfBanks;

    bool checkIfOccured (MemoryDump& allStates);
    unsigned findBankWithMostBlocks();
    void redistribution();
    void saveState (MemoryDump& savePlace);


};

#endif // MEMORY_H
