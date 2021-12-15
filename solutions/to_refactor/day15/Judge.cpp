#include "Judge.h"

void Judge::TryToJudge()
{
	if (!m_first_generator_values.empty() && !m_second_generator_values.empty())
	{
		++m_checked_pairs;

		long long next_val_A = m_first_generator_values.front();
		m_first_generator_values.pop();
		long long next_val_B = m_second_generator_values.front();
		m_second_generator_values.pop();

		long long mask = (1 << 16) - 1;
		if ((next_val_A & mask) == (next_val_B & mask))
			++m_score;
	}
}

void Judge::UpdateValues(Generator* which_one, long long value)
{
	if (which_one == &m_first_generator)
		m_first_generator_values.push(value);
	else
		m_second_generator_values.push(value);
}