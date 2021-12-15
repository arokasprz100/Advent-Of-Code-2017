#ifndef Judge_h
#define Judge_h

#include <queue>

class Generator;

class Judge
{
public:
	Judge (Generator& first_generator, Generator& second_generator):
		m_first_generator(first_generator), m_second_generator(second_generator), m_score(0), m_checked_pairs(0) {}

	void TryToJudge();

	unsigned GetScore() const { return m_score; }

	int GetNumberOfChecked() const { return m_checked_pairs; }

	void UpdateValues(Generator* which_one, long long value);

private:
	Generator& m_first_generator;
	Generator& m_second_generator;

	std::queue<long long> m_first_generator_values;
	std::queue<long long> m_second_generator_values;

	unsigned m_score;

	int m_checked_pairs = 0;

};


#endif