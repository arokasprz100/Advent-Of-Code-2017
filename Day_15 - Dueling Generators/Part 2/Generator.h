#ifndef Generator_h
#define Generator_h

#include "Judge.h"

class Generator
{
public:

	Generator (long long factor, long long starting_value, int condition) : 
		m_factor(factor), m_current_value(starting_value), m_condition(condition) {}

	void TryToSendValueToJudge(Judge& where_to_send) 
	{
		if (m_current_value % m_condition == 0)
			where_to_send.UpdateValues(this, m_current_value);
	}

	void GenerateNewValue()
	{
		m_current_value = (m_current_value * m_factor) % m_divisor;
	}

private:

	const long long m_factor;

	long long m_current_value;

	int m_condition;

	static const long long m_divisor = 2147483647;

};

#endif