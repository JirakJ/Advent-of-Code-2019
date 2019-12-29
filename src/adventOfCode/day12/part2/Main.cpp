#include "../helper.hpp"

int main()
{
	std::fstream in("input.in", std::fstream::in);
	std::fstream out("output.out", std::fstream::out);
	std::vector<std::vector<int>> initialMoonsPos(3);
	std::vector<Coordinate> moons;

	readInputPart2(in, moons, initialMoonsPos);

	out << findSteps(moons, initialMoonsPos);

	in.close();
	out.close();
}