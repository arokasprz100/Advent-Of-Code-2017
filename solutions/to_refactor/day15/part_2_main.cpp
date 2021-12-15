#include <iostream>

#include "Judge.h"
#include "Generator.h"

int main()
{
	Generator generatorA (16807, 116, 4);
	Generator generatorB (48271, 299, 8);

	Judge judgeForAB(generatorA, generatorB);

	while (true)
	{
		generatorA.GenerateNewValue();
		generatorA.TryToSendValueToJudge(judgeForAB);

		generatorB.GenerateNewValue();
		generatorB.TryToSendValueToJudge(judgeForAB);

		judgeForAB.TryToJudge();

		if (judgeForAB.GetNumberOfChecked() == 5000000)
			break;
	}

	std::cout << "Final score: " << judgeForAB.GetScore() << std::endl;
}