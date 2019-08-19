#include "pch.h"
#include "Map.h"

using namespace std;

Map::Map(int floor)
{
	random_device rng;
	sizeX = min((int)(rng() % 40 + 40 + (floor)), 230);
	sizeY = min((int)(rng() % 12 + 10 + (floor)), 50);
	bitMap.resize(sizeY + 1);
	for (int i = 0; i < sizeY + 1; i++) {
		bitMap[i].resize(sizeX + 1);
	}
	divList.push_back(Division(0, sizeY, 0, sizeX));
}

Map::Map(int x, int y)
{
	sizeX = x;
	sizeY = y;
}

Map::~Map()
{
}

void Map::Display()
{
	int i;
	for (i = 0; i < sizeY; i++) {
		cout << map[i] << endl;
	}
}

void Map::Divide()
{
	random_device rng;
	while (divList.back().outer.measure > 50) {
		Division div = divList.back();
		divList.pop_back();

		if (rng()%2) {
			int divArea = div.outer.width - (MIN_DIVISION_SIDE * 2);
			if (divArea > 0) {
				int divPoint = rng() % min(divArea, MAX_DIVISION_SIDE) + MIN_DIVISION_SIDE;

				divList.push_back(Division(div.outer.top, div.outer.bottom,
					div.outer.left, div.outer.left + divPoint));
				divList.push_back(Division(div.outer.top, div.outer.bottom,
					div.outer.left + divPoint, div.outer.right));
			}
			else {
				divList.push_back(div);
				return;
			}
		}
		else {
			int divArea = div.outer.height - (MIN_DIVISION_SIDE * 2);
			if (divArea > 0) {
				int divPoint = rng() % min(divArea, MAX_DIVISION_SIDE) + MIN_DIVISION_SIDE;

				divList.push_back(Division(div.outer.top, div.outer.top + divPoint,
					div.outer.left, div.outer.right));
				divList.push_back(Division(div.outer.top + divPoint, div.outer.bottom,
					div.outer.left, div.outer.right));
			}
			else {
				divList.push_back(div);
				return;
			}
		}
	}
}

void Map::CreateBitMap()
{
	for (Division div : divList) {
		for (int j = div.room.top; j < div.room.bottom; j++) {
			for (int k = div.room.left; k < div.room.right; k++) {
				bitMap[j][k] = 1;
			}
		}
	}

	for (int i = 1; i < pathList.size(); i += 2) {
		for (int j = i + 2; j < pathList.size(); j += 2) {
			if (pathList[i].first == pathList[j].first) {
				for (int k = min(pathList[i - 1].first, pathList[i].first);
					k <= max(pathList[i - 1].first, pathList[i].first); k++) {

					bitMap[pathList[i].second][k] = 1;
				}
				for (int k = min(pathList[j - 1].first, pathList[j].first);
					k <= max(pathList[j - 1].first, pathList[j].first); k++) {

					bitMap[pathList[j].second][k] = 1;
				}
				for (int k = min(pathList[i].second, pathList[j].second);
					k <= max(pathList[i].second, pathList[j].second); k++) {

					bitMap[k][pathList[i].first] = 1;
				}

			}
			else if (pathList[i].second == pathList[j].second) {
				for (int k = min(pathList[i - 1].second, pathList[i].second);
					k <= max(pathList[i - 1].second, pathList[i].second); k++) {

					bitMap[k][pathList[i].first] = 1;
				}
				for (int k = min(pathList[j - 1].second, pathList[j].second);
					k <= max(pathList[j - 1].second, pathList[j].second); k++) {

					bitMap[k][pathList[j].first] = 1;
				}
				for (int k = min(pathList[i].first, pathList[j].first);
					k <= max(pathList[i].first, pathList[j].first); k++) {

					bitMap[pathList[i].second][k] = 1;
				}
			}
		}
	}
}

void Map::CreateMap()
{
	map.resize(sizeY + 1);
	for (int i = 0; i < sizeY + 1; i++) {
		map[i] = string(sizeX + 1, ' ');
		for (int j = 0; j < sizeX + 1; j++) {
			if (bitMap[i][j] == 1) {
				Add(j, i, ' ');
			}
			else {
				Add(j, i, '#');
			}
		}
	}
}

void Map::CreatePath()
{
	for (Division &div : divList) {
		int centerX = div.room.width / 2 + div.room.left;
		int centerY = div.room.height / 2 + div.room.top;
		if (div.outer.top != 0) {
			pathList.push_back(pair<int, int>(centerX, div.room.top));
			pathList.push_back(pair<int, int>(centerX, div.outer.top));
		}
		if (div.outer.bottom != sizeY) {
			pathList.push_back(pair<int, int>(centerX, div.room.bottom));
			pathList.push_back(pair<int, int>(centerX, div.outer.bottom));
		}
		if (div.outer.left != 0) {
			pathList.push_back(pair<int, int>(div.room.left, centerY));
			pathList.push_back(pair<int, int>(div.outer.left, centerY));
		}
		if (div.outer.right != sizeX) {
			pathList.push_back(pair<int, int>(div.room.right, centerY));
			pathList.push_back(pair<int, int>(div.outer.right, centerY));
		}
	}
}
int Map::MapCheck(pair<int, int> position)
{
	switch (map[position.second][position.first]) {
	case ' ':
		return 0;
	case '#':
		return 1;
	case 'E':
		return 2;
	case '%':
		return 3;
	default:
		return 4;
	}
}