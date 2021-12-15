#include <fstream>
#include <iostream>



unsigned process(std::ifstream& input)
{
	unsigned score = 0;
	int depth = 0;
	char next_char;
	unsigned total_garbage = 0;
	while (!input.eof())
	{
		input.get(next_char);

		if (input.eof())
			break;

		if (next_char == '!'){
			input.get(next_char);
			continue;
		}

		if (next_char == '<'){
			while (true){
				input.get(next_char);
				if (next_char == '!'){
					input.get(next_char);
					continue;
				}
				if (next_char == '>')
					break;

				++total_garbage;
			}
		}

		if (next_char == '{')
			++depth;

		if (next_char == '}')
		{
			score += depth;
			--depth;
		}
		
	}
	std::cout << "Total garbage: " << total_garbage << std::endl;
	return score;
}

int main()
{
	std::ifstream file("input.txt");
	std::cout << process(file) << std::endl;
		
	return 0;
}