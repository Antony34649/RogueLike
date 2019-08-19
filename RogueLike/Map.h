#pragma once

#include "Division.h"
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

#define MIN_DIVISION_MEASURE 12
#define MIN_DIVISION_SIDE 7
#define MAX_DIVISION_SIDE 10

class Map
{
private:
	std::vector<std::string> map;
public:
	int sizeX, sizeY;
	std::vector<Division> divList;
	std::vector<std::vector<int>> bitMap;
	std::vector<Rect> roomList;
	std::vector < std::pair<int, int>> pathList;

	Map(int);
	Map(int x, int y);
	~Map();
	std::vector<std::string> GetMap() { return map; }
	void Display();
	char Check(int x, int y) { return map[y][x]; }
	void Add(int x, int y, char ch) { map[y][x] = ch; }
	void Divide();
	void CreateBitMap();
	void CreateMap();
	int MapCheck(std::pair<int, int>);
	void CreatePath();
};

