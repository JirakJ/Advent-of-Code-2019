#include "../helper.hpp"

int main()
{
	std::fstream in("input.in", std::fstream::in);
	std::fstream out("output.out", std::fstream::out);
	std::vector<long long> integers;
	long long input = -1;
	int x = 5, y = 5;

	readInput(in, integers);

	while (true)
	{
		if ((!intCodeProgram(out, integers, input, x, y)) || (!intCodeProgram(out, integers, input, x, y + 99)))
		{
			x++;
			while (!intCodeProgram(out, integers, input, x, y))
			{
				y++;
			}
			continue;
		}

		if (!intCodeProgram(out, integers, input, x + 99, y))
		{
			y++;
			continue;
		}

		out << x * 10000 + y;
		break;
	}
	
	in.close();
	out.close();
}