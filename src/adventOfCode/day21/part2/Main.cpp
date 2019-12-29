#include "../helper.hpp"

int main()
{
	std::fstream in("input.in", std::fstream::in);
	std::fstream out("output.out", std::fstream::out);
	std::vector<long long> integers;
	std::queue<long long> input;

	// ((!A || !B || !C) && D) && (H || E)
	// If I can jump again or walk forward one tile after jumping than jump
	initQueue("NOT A T\n", input);
	initQueue("OR T J\n", input);
	initQueue("NOT B T\n", input);
	initQueue("OR T J\n", input);
	initQueue("NOT C T\n", input);
	initQueue("OR T J\n", input);
	initQueue("AND D J\n", input);

	initQueue("NOT H T\n", input);
	initQueue("NOT T T\n", input);
	initQueue("OR E T\n", input);
	initQueue("AND T J\n", input);

	initQueue("RUN\n", input);

	readInput(in, integers);
	intCodeProgram(out, integers, input);

	in.close();
	out.close();
}