#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <algorithm>
#include <math.h>
#include <stack>
#include <bitset>


void readInput(std::fstream& in, std::vector<std::vector<char>>& map)
{
	std::vector<char> v;
	std::string line;

	while (std::getline(in, line))
	{
		for (int i = 0; i < line.size(); i++)
		{
			v.push_back(line[i]);
		}
		map.push_back(v);
		v.clear();
	}
}


bool checkInMap(int x, int y, int lines, int columns)
{
	return x >= 0 && y >= 0 && x < lines && y < columns;
}


int findBiodiversityRating(std::vector<std::vector<char>>& map)
{
	std::vector< std::vector<std::vector<char>>> visit;
	std::vector<std::vector<char>> currMap(map);
	std::vector<int> dx{ -1,0,0,1 };
	std::vector<int> dy{ 0,-1,1,0 };
	int bugs{ 0 };

	visit.push_back(map);
	while (true)
	{
		for (int l = 0; l < 5; l++)
		{
			for (int c = 0; c < 5; c++)
			{
				bugs = 0;
				for (int k = 0; k < 4; k++)
				{
					if (checkInMap(l + dx[k], c + dy[k], 5, 5))
					{

						if (map[l + dx[k]][c + dy[k]] == '#')
						{
							bugs++;
						}
					}
				}

				if ((map[l][c] == '#') && (bugs != 1))
				{
					currMap[l][c] = '.';
				}
				if ((map[l][c] == '.') && ((bugs == 1) || (bugs == 2)))
				{
					currMap[l][c] = '#';
				}
			}
		}

		map = currMap;

		for (int it = 0; it < visit.size(); it++)
		{
			if (visit[it] == map)
			{
				int sum = 0;
				for (int l = 0; l < 5; l++)
				{
					for (int c = 0; c < 5; c++)
					{
						if (map[l][c] == '#')
						{
							sum += pow(2, l * 5 + c);
						}
					}
				}
				return sum;
			}
		}

		visit.push_back(map);
	}
}


int numberBugs(std::vector<std::vector<char>>& map, const int minutes)
{
	std::vector<std::vector<char>> emptyGrid(5, std::vector<char>(5, '.'));
	std::vector< std::vector<std::vector<char>>> levels;
	std::vector<int> dx{ -1,0,0,1 };
	std::vector<int> dy{ 0,-1,1,0 };
	int bugs{ 0 }, countBugs{ 0 };
	int lowestDepth{ minutes }, highestDepth{ minutes + 2 };
	emptyGrid[2][2] = '?';
	map[2][2] = '?';

	for (int i = 0; i <= minutes; i++)
	{
		levels.push_back(emptyGrid);
	}
	levels.push_back(map);
	for (int i = 0; i <= minutes; i++)
	{
		levels.push_back(emptyGrid);
	}

	std::vector< std::vector<std::vector<char>>> currLevels(levels);
	std::vector<std::vector<char>> currMap(emptyGrid);

	for (int it = 0; it < minutes; it++)
	{
		for (int lvl = lowestDepth; lvl <= highestDepth; lvl++)
		{
			currMap = levels[lvl];
			for (int i = 0; i < 5; i++)
			{
				for (int j = 0; j < 5; j++)
				{
					bugs = 0;
					for (int k = 0; k < 4; k++)
					{
						if (checkInMap(i + dx[k], j + dy[k], 5, 5))
						{
							if (levels[lvl][i + dx[k]][j + dy[k]] == '?')
							{
								switch (k)
								{
								case 0:
									for (int l = 0; l < 5; l++)
									{
										if (levels[lvl + 1][4][l] == '#')
										{
											bugs++;
										}
									}
									break;

								case 1:
									for (int l = 0; l < 5; l++)
									{
										if (levels[lvl + 1][l][4] == '#')
										{
											bugs++;
										}
									}
									break;

								case 2:
									for (int l = 0; l < 5; l++)
									{
										if (levels[lvl + 1][l][0] == '#')
										{
											bugs++;
										}
									}
									break;

								case 3:
									for (int l = 0; l < 5; l++)
									{
										if (levels[lvl + 1][0][l] == '#')
										{
											bugs++;
										}
									}
									break;
								}
							}
							else
							{
								if (levels[lvl][i + dx[k]][j + dy[k]] == '#')
								{
									bugs++;
								}
							}
						}
						else
						{
							if (levels[lvl - 1][2 + dx[k]][2 + dy[k]] == '#')
							{
								bugs++;
							}
						}
					}

					if ((levels[lvl][i][j] == '#') && (bugs != 1))
					{
						currMap[i][j] = '.';
					}
					if ((levels[lvl][i][j] == '.') && ((bugs == 1) || (bugs == 2)))
					{
						currMap[i][j] = '#';
					}
				}
			}
			currLevels[lvl] = currMap;
		}
		lowestDepth--;
		highestDepth++;
		levels = currLevels;
	}

	for (int i = 0; i <= minutes * 2 + 1; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int k = 0; k < 5; k++)
			{
				if (levels[i][j][k] == '#')
				{
					countBugs++;
				}
			}
		}
	}

	return countBugs;
}