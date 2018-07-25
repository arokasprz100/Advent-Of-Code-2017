#include <fstream>
#include <iostream>
#include <vector>

int main()
{
	std::vector<int> ranges;
	std::vector<int> scanner_positions;

	std::ifstream file("input.txt");
	while (file.good())
	{
		int val;
		file >> val;
		if (!file.good())
			break;
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

	int delay = 0;
	for (;;)
	{

		bool is_safe = true;
		int current_position = -1;

		std::cout << "Delay = " << delay << std::endl;

		if (delay > 0){
			for (int j = 0; j < ranges.size(); ++j)
			{
				if (ranges.at(j) == 0)
					continue;
				if (( (delay - 1) / ((ranges.at(j) - 1))) % 2 == 0 )
					++scanner_positions.at(j);
				else 
					--scanner_positions.at(j);
			}
		}
		
		std::vector<int> scanner_positions_temp = scanner_positions;

		for (int i = 0; i < ranges.size(); ++i)
		{
			++current_position;
			if (scanner_positions_temp.at(current_position) == 0 and ranges.at(current_position) != 0){
				is_safe = false;
				break;
			}

			for (int j = 0; j < ranges.size(); ++j)
			{
				if (ranges.at(j) == 0)
					continue;
				if (((current_position + delay) / (ranges.at(j)-1)) % 2 == 0 )
					++scanner_positions_temp.at(j);
				else 
					--scanner_positions_temp.at(j);

			}
		}
		
		if (is_safe == true)
			break;
		++delay;
	}

	std::cout << "Minimum delay: " << delay << std::endl;

	return 0;
}