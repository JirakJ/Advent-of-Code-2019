#include "../helper.hpp"

int main()
{
	std::fstream in("input.in", std::fstream::in);
	std::fstream out("output.out", std::fstream::out);
	std::vector<long long> integers;
	long long input = -1;
	int pointsAffected = 0;

	readInput(in, integers);

	for (int x = 0; x < 50; x++)
	{
		for (int y = 0; y < 50; y++)
		{
			if (intCodeProgram(out, integers, input, x, y))
			{
				pointsAffected++;
			}
		}
	}
	
	out << pointsAffected;

	in.close();
	out.close();
}