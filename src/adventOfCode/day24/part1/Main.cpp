#include "../helper.hpp"

int main()
{
	std::fstream in("input.in", std::fstream::in);
	std::fstream out("output.out", std::fstream::out);
	std::vector<std::vector<char>> map;

	readInput(in, map);

	out << findBiodiversityRating(map);

	in.close();
	out.close();
}