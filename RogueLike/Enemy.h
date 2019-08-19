#pragma once

#include "Map.h"
#include "Rect.h"
#include <string>
#include <random>
#include <algorithm>
#include <sstream>

class Enemy
{
public:
	int x, y, hp, atk, def;
	int minAreaX, maxAreaX, minAreaY, maxAreaY;

	Enemy();
	Enemy(Rect&, Map&, int);
	~Enemy();

	std::string StatusDisplay();
	void Move(Map&);
	int Damage(int);
	bool Battle(int, int, Map&);
};

