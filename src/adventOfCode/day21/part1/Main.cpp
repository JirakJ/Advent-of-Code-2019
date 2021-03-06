#include "../helper.hpp"

int main()
{
	std::fstream in("input.in", std::fstream::in);
	std::fstream out("output.out", std::fstream::out);
	std::vector<long long> integers;
	std::queue<long long> input;

	// (!A || !B || !C) && D
	// If we have a hole (one tile away or two tiles away or three tiles away) and the forth tile where
	// we will land after jumping is ground than we jump
	initQueue("NOT A T\n", input);
	initQueue("OR T J\n", input);
	initQueue("NOT B T\n", input);
	initQueue("OR T J\n", input);
	initQueue("NOT C T\n", input);
	initQueue("OR T J\n", input);
	initQueue("AND D J\n", input);

	initQueue("WALK\n", input);
	
	readInput(in, integers);
	intCodeProgram(out, integers, input);

	in.close();
	out.close();
}