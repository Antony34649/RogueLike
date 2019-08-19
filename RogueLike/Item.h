#pragma once

#include <random>
#include <string>
#include "Map.h"
#include "Rect.h"

class Item
{
public:
	int x, y, type, hp=0, atk=0, def=0, hng=0;
	std::string isEquiped;
	char visual;
	bool isPicked;

	Item();
	Item(int);
	Item(Rect&, Map&, int);
	~Item();
	void Register(Map&);
	void DecideEffect(int);
};

