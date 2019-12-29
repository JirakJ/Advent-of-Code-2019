#include "../helper.hpp"

int main()
{
	std::fstream in("input.in", std::fstream::in);
	std::fstream out("output.out", std::fstream::out);
	std::vector<Coordinate> moons;
	int maxSteps = 1000;

	readInput(in, moons);
	out << findTotalEnergy(moons, maxSteps);

	in.close();
	out.close();
}