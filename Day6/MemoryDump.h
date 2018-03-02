#ifndef MEMORYDUMP_H
#define MEMORYDUMP_H

#include <vector>
#include <map>


class MemoryDump
{
public:
    static MemoryDump& getInstance();
    void addState (std::vector<int> stateToAdd);
    void clearMemoryHistory();

    friend class Memory;

private:
    MemoryDump() :
        m_numberOfStates(0) {}

    std::map<std::vector<int>, int> m_allStates;
    unsigned m_numberOfStates;

};

#endif // MEMORYDUMP_H
