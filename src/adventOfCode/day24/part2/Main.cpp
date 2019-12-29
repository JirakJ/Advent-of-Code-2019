#include "../helper.hpp"

int main()
{
	std::fstream in("input.in", std::fstream::in);
	std::fstream out("output.out", std::fstream::out);
	std::vector<std::vector<char>> map;
	const int minutes = 200;

	readInput(in, map);

	out << numberBugs(map, minutes);

	in.close();
	out.close();
}