#include <iostream>

int main()
{
	/// From puzzle input
	const long long generator_A_start_val = 116;
	const long long generator_B_start_val = 299;

	const long long generator_A_factor = 16807;
	const long long generator_B_factor = 48271;

	const long long divisor = 2147483647;

	int score = 0; 

	const int number_of_pairs = 40000000; // 40 million

	long long previous_A = generator_A_start_val;
	long long previous_B = generator_B_start_val;

	for (int i = 0; i < number_of_pairs; ++i)
	{
		long long mask = (1 << 16) - 1;

		long long next_val_A = (previous_A * generator_A_factor) % divisor;
		long long next_val_B = (previous_B * generator_B_factor) % divisor;


		if ((next_val_A & mask) == (next_val_B & mask))
			++score;

		previous_A = next_val_A;
		previous_B = next_val_B;
	}

	std::cout << "Judge's final count is " << score << std::endl;

	return 0;
}