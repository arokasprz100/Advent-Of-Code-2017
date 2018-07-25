#include "ListOfJumps.h"

#include <iostream>
#include <fstream>
#include <cstdlib>

int main()
{
    std::ifstream inputFile("input.txt");
    if (!inputFile.is_open())
    {
        std::cout<<"Could not open input file"<<std::endl;
        exit(-1);
    }

    ListOfJumps jumps(inputFile);
    ///jumps.printVector();
    jumps.traverseVector(false);
    std::cout<<"(Part 1) Exit reached in "<<jumps.getJumpCounter()<<" jumps."<<std::endl;

    jumps.resetJumper();
    jumps.traverseVector(true);
    std::cout<<"(Part 2) Exit reached in "<<jumps.getJumpCounter()<<" jumps."<<std::endl;

    inputFile.close();


    return 0;
}
