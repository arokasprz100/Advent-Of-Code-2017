#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>

int main()
{
	constexpr int number_of_rounds = 64;
	constexpr int list_size = 256;
	int list[list_size];
	for (int i = 0; i < list_size; ++i)
		list[i] = i;

	unsigned current_position = 0;
	unsigned skip_size = 0;

	std::ifstream file("input.txt");

	std::vector<int> distances;

	while (file.good())
	{
		char current_char = file.get();
		if (!file.good() or current_char == '\n' or current_char == '\r')
			break;
		distances.push_back(static_cast<int>(current_char));
	}


	std::vector<int> additional_distances = {17, 31, 73, 47, 23};
	distances.insert(distances.end(), additional_distances.begin(), additional_distances.end());

	for (int i = 0; i < number_of_rounds; ++i)
	{
		for (auto current_var : distances)
		{
			std::vector<int> temp;

			for (int j = 0; j < current_var; ++j)
				temp.push_back(list[(current_position + j) % list_size]);

			std::reverse(temp.begin(), temp.end());

			for (int j = 0; j < current_var; ++j)
				list[(current_position + j) % list_size] = temp.at(j);

			current_position = (current_position + current_var + skip_size) % list_size;

			++skip_size;
		}
	}

	std::vector<int> dense_hash;

	for (int i = 0; i < 16; ++i)
	{
		int current_hash = list[i * 16];
		for (int j = 1; j < 16; ++j)
			current_hash ^= list[i * 16 + j];
		dense_hash.push_back(current_hash);
	}

	
	for (auto digit : dense_hash)
		std::cout << std::hex << std::setw(2) << std::setfill('0') << digit;
	std::cout << std::endl;

	return 0;
}