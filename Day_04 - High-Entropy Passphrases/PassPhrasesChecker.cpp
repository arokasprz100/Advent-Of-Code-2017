#include "PassPhrasesChecker.h"

#include <sstream>
#include <iostream>
#include <algorithm>

PassPhrasesChecker::PassPhrasesChecker()
{
    m_counterPartOne = 0;
    m_counterPartTwo = 0;
}

void PassPhrasesChecker::checkFile (std::ifstream& file)
{
    std::string line;
    while (getline(file, line))
    {
        checkLineForPartOne(line) ? m_counterPartOne++ : m_counterPartOne;
        checkLineForPartTwo(line) ? m_counterPartTwo++ : m_counterPartTwo;
    }
}

bool PassPhrasesChecker::checkLineForPartOne (std::string lineToCheck)
{
    std::vector<std::string> vectorOfWords;
    std::string word;
    std::stringstream line(lineToCheck);
    while (getline(line, word, ' '))
        vectorOfWords.push_back(word);

    printVectorOfWords(vectorOfWords);

    for (unsigned i=0; i<vectorOfWords.size()-1; ++i)
    {
        for (unsigned j=i+1; j<vectorOfWords.size(); ++j)
        {
            if (vectorOfWords.at(i)==vectorOfWords.at(j))
                return false;
        }
    }

    return true;
}

bool PassPhrasesChecker::checkLineForPartTwo (std::string lineToCheck)
{
    std::vector<std::string> vectorOfWords;
    std::string word;
    std::stringstream line(lineToCheck);
    while (getline(line, word, ' '))
        vectorOfWords.push_back(word);

    sortLine(vectorOfWords);

    for (unsigned i=0; i<vectorOfWords.size()-1; ++i)
    {
        for (unsigned j=i+1; j<vectorOfWords.size(); ++j)
        {
            if (vectorOfWords.at(i)==vectorOfWords.at(j))
                return false;
        }
    }

    return true;



}

unsigned PassPhrasesChecker::getCounterForPartOne() const
{
    return m_counterPartOne;
}

unsigned PassPhrasesChecker::getCounterForPartTwo() const
{
   return m_counterPartTwo;
}

void PassPhrasesChecker::printVectorOfWords (const std::vector<std::string>& vectorOfWords) const
{
    for (unsigned i=0; i<vectorOfWords.size(); ++i)
        std::cout<<vectorOfWords.at(i)<<" ";
    std::cout<<std::endl;
}

void PassPhrasesChecker::sortLine (std::vector<std::string>& vectorToSort)
{
    for (unsigned i=0; i<vectorToSort.size(); ++i)
        sortWord(vectorToSort.at(i));
}


void PassPhrasesChecker::sortWord (std::string& word)
{
    std::sort(word.begin(), word.end());
}
