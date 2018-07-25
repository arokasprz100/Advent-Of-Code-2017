#include <iostream>
#include <fstream>
#include <algorithm>
#include <cwctype>

/*
	In this task, I use cube coordinates to keep track of child process position. 
	They are explained in detail here: 
	https://www.redblobgames.com/grids/hexagons/
	Basically it means that we look at our hex grid using three (instead of two)
	coordinates. Coordinates are connected like this:
	x + y + z = 0
	I calculate distance using following formula:
	d = max( abs(x2 - x1), abs(y2 - y1), abs(z2 - z1) )
*/


int calculate_distance(int x, int y, int z)
{
	x = abs(x);
	y = abs(y);
	z = abs(z);
	return ((x > y) ? ( x > z ? x : z) : ( y > z ? y : z));
}

int main()
{
	int x = 0, y = 0, z = 0;
	std::ifstream file ("input.txt");
	std::string next_move;
	int furthest = 0;
	
	while (file.good())
	{
		std::getline (file, next_move, ',');
		if (!file.good()) /// last move does not end with ',' in provided input file
			next_move.erase(std::remove_if(next_move.begin(), next_move.end(), iswspace), next_move.end());

		if (next_move == "n") { ++x ; --y; }
		else if (next_move == "s") { --x; ++y; }
		else if (next_move == "ne") { ++z; --y; }
		else if (next_move == "nw") { --z; ++x; }
		else if (next_move == "se") { --x; ++z; }
		else if (next_move == "sw") { ++y; --z; }

		int new_distance = calculate_distance(x, y, z);
		if (new_distance > furthest)
			furthest = new_distance;
	}


	std::cout << "x = " << x << " y = " << y << " z = " << z << std::endl;

	std::cout << "Minimal distance: " << calculate_distance(x, y, z) << std::endl;
	std::cout << "Furthest distance: " << furthest << std::endl;

	return 0;
}