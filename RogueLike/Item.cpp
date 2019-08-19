#include "pch.h"
#include "Item.h"

using namespace std;

Item::Item()
{
}

Item::Item(int t)
{
	type = t;
	DecideEffect(1);
}

Item::Item(Rect &rect, Map &map, int floor)
{
	random_device rng;
	type = rng() % 3;
	DecideEffect(floor);
	isPicked = false;

	do {
		x = rng() % rect.width + rect.left;
		y = rng() % rect.height + rect.top;
	} while (map.Check(x, y) != ' ');
	Register(map);
}

Item::~Item()
{
}

void Item::Register(Map &map)
{
	map.Add(x, y, visual);
}

void Item::DecideEffect(int floor)
{
	random_device rng;
	switch (type) {
	case 0:
		visual = 'H';
		hp = rng() % 6 + floor;
		hng = rng() % 6 + floor + 5;
		break;
	case 1:
		visual = 'A';
		atk = rng() % 4 + floor;
		break;
	case 2:
		visual = 'D';
		def = rng() % 4 + floor;
		break;
	}
}