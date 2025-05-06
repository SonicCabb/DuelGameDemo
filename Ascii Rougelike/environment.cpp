#include "Environment.h"



Environment::Environment()
{

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				_defMap[i][j][k] = '&';
				_map[i][j][k] = '&';
			}
		}
	}
	_levelID = -1;
}


Environment::~Environment()
{
}
