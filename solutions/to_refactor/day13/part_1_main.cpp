#include <fstream>
#include <iostream>
#include <vector>

/*
	In this solution I have vector of ints that represents ranges - so
	it is basically parsed data from input file. I also have second 
	vector of ints that stores current scanners positions. The biggest 
	(but still quite easy) problem I had to solve was to determine if
	I need to move scanner up or down at given time - I did not want
	to solve it by adding vector of bools. So I noticed that if I divide
	current step (that is also current position) by (layers_range - 1)
	I get a result that can determine it. If result is even, I need to
	add to current scanner position, otherwise, I need to subtract 1 form
	it. If there is no scanner at current position (so range is 0), 
	I simply skip.
*/

int main()
{
	std::vector<int> ranges;
	std::vector<int> scanner_positions;

	std::ifstream file("input.txt");
	while (file.good())
	{
		int val;
		file >> val;
		while (ranges.size() <= val)
			ranges.push_back(0);
		char delim; // ignore ',' character
		file >> delim;
		int range;
		file >> range;
		ranges.at(val) = range; 
	}

	for (int i = 0; i < ranges.size(); ++i){
		scanner_positions.push_back(0);
	}

	int severity = 0;
	int current_position = -1;
	for (int i = 0; i < ranges.size(); ++i)
	{
		++current_position;
		if (scanner_positions.at(current_position) == 0)
			severity += current_position * ranges.at(current_position);
		for (int j = 0; j < ranges.size(); ++j)
		{
			if (ranges.at(j) == 0)
				continue;
			if ((current_position / (ranges.at(j)-1)) % 2 == 0 )
				++scanner_positions.at(j);
			else --scanner_positions.at(j);

		}
	}

	std::cout << "Severity = " << severity << std::endl;

	return 0;
}