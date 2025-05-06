#include <string>
#include <vector>

#pragma once
class Environment
{
public:
	
	Environment();
	~Environment();

	//getters
	char getChar(int y, int x) { return _map[y][x][0]; }			//returns the currect character value at a coordinate
	char getDefChar(int y, int x) { return _defMap[y][x][0]; }		//returns the original character value at a coordinate

private:

	char _defMap[100][100][3];		//hold information about the default map data
	char _map[100][100][3];			//holds information about the current map data

	int _levelID;



};

