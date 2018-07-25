#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>
#include <climits>


int from_string (const std::string& oper)
{
	std::stringstream sstr(oper);
	int val;
	sstr >> val;
	return val;
}


int main()
{
	std::fstream input_file("input.txt");

	std::string changed;
	std::string action;
	int value;
	std::string conditionLhs;
	std::string condition;
	std::string conditionRhs;
	std::string if_statement;

	std::map<std::string, int> registers;

	int total_max = INT_MIN;

	while (!input_file.eof())
	{
		input_file >> changed >> action >> value >> if_statement 
					>> conditionLhs >> condition >> conditionRhs;

		if (input_file.eof())
			break;

		std::cout << changed << " " << action << " " << value << " " << if_statement << " " << 
					conditionLhs << condition << conditionRhs << std::endl;



		bool result = false;

		if (condition == "==")
			result = (registers[conditionLhs] == from_string(conditionRhs));
		else if (condition == "!=")
			result = (registers[conditionLhs] != from_string(conditionRhs));
		else if (condition == "<=")
			result = (registers[conditionLhs] <= from_string(conditionRhs));
		else if (condition == ">=")
			result = (registers[conditionLhs] >= from_string(conditionRhs));
		else if (condition == "<")
			result = (registers[conditionLhs] < from_string(conditionRhs));
		else if (condition == ">")
			result = (registers[conditionLhs] > from_string(conditionRhs));

		if (result)
			registers[changed] += (action == "inc" ? value : -value);

		if (registers[changed] > total_max)
			total_max = registers[changed];


	}

	auto max = std::max_element(registers.begin(), registers.end(), [](std::map<std::string, int>::value_type& a, std::map<std::string, int>::value_type& b){ return a.second < b.second; });

	std::cout << (*max).first << " : " << (*max).second << std::endl;

	std::cout << "Total max : " << total_max << std::endl;

}