#ifndef DAY1FUNCTIONS_H
#define DAY1FUNCTIONS_H

#include <vector>
#include <fstream>

/// This function is designed to work with input provided for this Advent Of Code task
/// It takes reference to an empty vector and a file that served as an input
/// Since input file contains only digits, it copies those digits into given vector
/// This function returns reference to filled vector - its the same vector that user passes
/// as a first argument
std::vector<int>& copyFileToVector (std::vector<int> & destination, std::fstream& inputFile);

/// This function simply prints all numbers stored in vector
void printVector (std::vector<int>& printedVector);

/// Solution of part I
/// This function calculates a sum of numbers in vector in a specific way
/// It sums only digits that match the next digit in vector
/// The list is circular, so the digit after the last digit is the first
/// digit (with index 0)
unsigned int sumInPartOne (std::vector<int>& vectorOfNumbers);

/// Solution of part II
/// This function calculates a sum of numbers in given vector in a specific way
/// It sums only those digits that match the digit that is halfway around the circular list
/// For example, if vector contains 10 elements, the first digit will be counted
/// only if the digit 0+ 10/2 (that is 5) steps forward is the same digit
/// The function takes reference to vector of ints as an input
unsigned int sumInPartTwo (std::vector<int>& vectorOfNumbers);

#endif // DAY1FUNCTIONS_H
