#include "MemoryDump.h"
#include <iostream>

MemoryDump& MemoryDump::getInstance()
{
    static MemoryDump instance;

    return instance;
}

void MemoryDump::clearMemoryHistory()
{
    m_allStates.clear();
    m_numberOfStates = 0;
}

void MemoryDump::addState (std::vector<int> stateToAdd)
{
    std::map<std::vector<int>,int>::iterator it = m_allStates.begin();
    m_allStates.insert (it, std::pair<std::vector<int>, int>(stateToAdd, m_numberOfStates));
    m_numberOfStates++;
}
