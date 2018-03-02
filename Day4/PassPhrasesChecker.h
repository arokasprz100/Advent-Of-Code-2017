#ifndef PASSPHRASESCHECKER_H
#define PASSPHRASESCHECKER_H

#include <string>
#include <vector>
#include <fstream>


class PassPhrasesChecker
{
public:
    PassPhrasesChecker();
    void checkFile (std::ifstream& file);
    unsigned getCounterForPartOne() const;
    unsigned getCounterForPartTwo() const;

protected:
    bool checkLineForPartOne (std::string lineToCheck);
    bool checkLineForPartTwo (std::string lineToCheck);
    void printVectorOfWords (const std::vector<std::string>& vectorOfWords) const;
    void sortLine (std::vector<std::string>& vectorToSort);
    void sortWord (std::string& word);

private:
    unsigned m_counterPartOne;
    unsigned m_counterPartTwo;
};

#endif // PASSPHRASESCHECKER_H
