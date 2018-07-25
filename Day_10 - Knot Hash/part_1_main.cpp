#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>

int main()
{
	constexpr int list_size = 256;
	int list[list_size];
	for (int i = 0; i < list_size; ++i)
		list[i] = i;

	unsigned current_position = 0;
	unsigned skip_size = 0;

	std::ifstream file("input.txt");
	std::string file_content;
	file >> file_content; 
	std::replace(file_content.begin(), file_content.end(), ',', ' ');

	std::cout << file_content << std::endl;

	std::stringstream file_stream(file_content);

	while (file_stream.good())
	{
		int current_var;
		file_stream >> current_var;

		std::vector<int> temp;
		for (int i = 0; i < current_var; ++i)
			temp.push_back(list[(current_position + i) % list_size]);

		std::reverse(temp.begin(), temp.end());

		for (int i = 0; i < current_var; ++i)
			list[(current_position + i) % list_size] = temp.at(i);

		current_position = (current_position + current_var + skip_size) % list_size;

		++skip_size;

	}

	std::cout << list[0] * list[1] << std::endl;

	return 0;
}