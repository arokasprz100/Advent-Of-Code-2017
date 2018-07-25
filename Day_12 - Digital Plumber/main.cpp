#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <set>


void visit_neighbours(int node, std::set<int>& visited, std::vector<std::vector<int>>& adjacency_list)
{
	for (std::vector<int>::const_iterator it = adjacency_list.at(node).begin(); it != adjacency_list.at(node).end(); ++it)
	{
		if (visited.find(*it) == std::end(visited) ){
			visited.insert(*it);
			visit_neighbours(*it, visited, adjacency_list);
		}
	}
}

int count_components(std::vector<std::vector<int>>& adjacency_list)
{
	int components = 0;
	std::set<int> counted_nodes;
	for (int i = 0; i < adjacency_list.size(); ++i)
	{
		if (counted_nodes.find(i) == std::end(counted_nodes)){
			counted_nodes.insert(i);
			++components;
			visit_neighbours(i, counted_nodes, adjacency_list);
		}
	}
	return components;
}

int main()
{
	std::vector<std::vector<int> > adjacency_list;
	std::ifstream file ("input.txt");
	while (file.good())
	{
		std::string file_line;
		std::getline(file, file_line);
		if (!file.good())
			break;

		std::replace(file_line.begin(), file_line.end(), ',', ' ');
		std::replace(file_line.begin(), file_line.end(), '<', ' ');
		std::replace(file_line.begin(), file_line.end(), '-', ' ');
		std::replace(file_line.begin(), file_line.end(), '>', ' ');

		adjacency_list.emplace_back();
		int current_node;
		std::stringstream line_stream(file_line);
		line_stream >> current_node;

		while (line_stream.good())
		{
			int neighbour;
			line_stream >> neighbour;
			adjacency_list.at(current_node).push_back(neighbour);
		}
	}

	file.close();

	std::set<int> visited;
	visited.insert(0);

	visit_neighbours(0, visited, adjacency_list);

	std::cout << "There are " << visited.size() << " such programs." << std::endl;
	std::cout << "There are " << count_components(adjacency_list) << " groups in total. " << std::endl;

	return 0;
}