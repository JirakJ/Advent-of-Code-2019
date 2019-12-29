#include "../helper.hpp"


int main()
{
	std::fstream in("input.in", std::fstream::in);
	std::fstream out("output.out", std::fstream::out);
	std::vector<long long> integers;
	bool input = true;

	readInput(in, integers);
	intCodeProgram(out, integers, input, true);

	in.close();
	out.close();
}