#pragma once

#include "Rect.h"
#include <random>
#include <list>

class Division
{
public:
	Rect outer;
	Rect room;
	std::vector<std::pair<int, int>> path;

	Division();
	Division(int, int, int, int);
	~Division();

	void CreateRoom();
	void CreatePath();
};

