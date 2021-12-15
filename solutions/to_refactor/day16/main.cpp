#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <sstream>

/*
 * Simulates one iteration of dance
 */
template <unsigned size>
void dance (char (&programs)[size], std::string& file_input)
{
	std::stringstream file(file_input);
	char temp_programs[size];

	while (file.good())
	{
		std::string move = "";
		std::getline(file, move, ',');
		if (move[0] == 's')
		{
			for (int i = 0; i < size; ++i)
				temp_programs[(i + std::atoi(&move[1])) % size] = programs[i];
			std::copy(std::begin(temp_programs), std::end(temp_programs), std::begin(programs));
		}
		else if (move[0] == 'x')
		{
			move.erase(0, 1);
			int pos1, pos2;
			std::stringstream move_stream (move);
			move_stream >> pos1;
			move_stream >> pos2;
			std::swap (programs[pos1], programs[pos2]);
		}
		else if (move[0] == 'p')
		{
			move.erase(0, 1);
			char char_to_swap_1, char_to_swap_2;
			std::stringstream move_stream (move);
			move_stream >> char_to_swap_1;
			move_stream >> char_to_swap_2;
			char* pos_of_char_1;
			char* pos_of_char_2;
			pos_of_char_1 = std::find(std::begin(programs), std::end(programs), char_to_swap_1);
			pos_of_char_2 = std::find(std::begin(programs), std::end(programs), char_to_swap_2);
			std::swap(*pos_of_char_1, *pos_of_char_2);
		}
	}
}

/*
 * Prints programs to standard output
 */
template<unsigned size>
void print_programs(char (&programs)[size])
{
	std::copy(std::begin(programs), std::end(programs), std::ostream_iterator<char>(std::cout));
	std::cout << std::endl;
}


int main()
{
	constexpr long long number_of_iterations = 1000000000L;
	constexpr unsigned array_size = 16;

	char programs[array_size];
	for (int i = 0; i < array_size; ++i)
		programs[i] = static_cast<char>( 'a' + i );

	std::string file_input;
	std::ifstream file_in("input.txt");
	file_in >> file_input;
	std::replace(file_input.begin(), file_input.end(), '/', ' ');

	char starting_configuration[array_size];
	std::copy(std::begin(programs), std::end(programs), std::begin(starting_configuration));

	/// Part I
	dance<array_size>(programs, file_input);
	std::cout << "Programs after one iteration of dance: " << std::endl;
	print_programs(programs);

	/// Part II
	int repeat_index = 0;
	for (long long j = 1; j < number_of_iterations; ++j)
	{
		dance<array_size>(programs, file_input);

		int same = 0;
		for (int k = 0; k < array_size; ++k)
			if (starting_configuration[k] == programs[k])
				++same;

		if (same == array_size){
			repeat_index = j + 1;
			break;
		}
	}

	long long last = number_of_iterations % repeat_index;
	for (int i = 0; i < last; ++i)
		dance<array_size>(programs, file_input);

	std::cout << "Programs after their complete dance: " << std::endl;
	print_programs(programs);
	
	return 0;
}