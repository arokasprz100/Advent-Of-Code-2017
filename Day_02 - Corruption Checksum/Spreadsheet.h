#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include <string>
#include <vector>
#include <fstream>


class Spreadsheet
{
public:
    Spreadsheet();
    void parseFile(std::fstream& fileToParse);
    unsigned getChecksumFromPartOne() const;
    unsigned getChecksumFromPartTwo() const;

protected:
    void parseLine (std::string line);
    void printLine(const std::vector<int>& valuesFromLine);
    unsigned findDiffMaxMin (std::vector<int>& numbers);
    unsigned findEventlyDivisible (std::vector<int>& numbers);

private:
    unsigned m_checksumPartOne;
    unsigned m_checksumPartTwo;

};

#endif // SPREADSHEET_H
