#ifndef LISTOFJUMPS_H
#define LISTOFJUMPS_H

#include <vector>
#include <fstream>

class ListOfJumps
{
public:
    ListOfJumps(std::ifstream& fileToParse);
    void printVector() const;
    void traverseVector(bool isPartTwo);
    int getJumpCounter() const;
    void resetJumper();


private:
    int m_jumperPosition;
    int m_jumpCounter;
    std::vector<int> m_jumps;


};

#endif // LISTOFJUMPS_H
