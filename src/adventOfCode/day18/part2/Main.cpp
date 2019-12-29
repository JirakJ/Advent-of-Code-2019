#include "../helper.hpp"

int main()
{
	std::fstream in("input.in", std::fstream::in);
	std::fstream out("output.out", std::fstream::out);
	std::vector<std::vector<char>> map;
	std::vector<std::bitset<letters>> visitedStart(4);
	std::vector<Coordinate> startCoordinates;

	readInputPart2(in, map, startCoordinates);

	ignoreDoors(map, visitedStart, startCoordinates);

	int fewestSteps = 0;
	for (int i = 0; i < 4; i++)
	{
		startCoordinates[i].visited = visitedStart[i].to_ullong();
		fewestSteps += BFS(map, startCoordinates[i]);
	}

	out << fewestSteps;

	in.close();
	out.close();
}