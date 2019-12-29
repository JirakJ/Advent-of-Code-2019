#include "../helper.hpp"

int main()
{
	std::fstream in("input.in", std::fstream::in);
	std::fstream out("output.out", std::fstream::out);
	std::vector<std::vector<char>> map;
	Coordinate startCoordinate;

	readInput(in, map, startCoordinate);

	out << BFS(map, startCoordinate);

	in.close();
	out.close();
}