#include "pch.h"
#include "Enemy.h"

using namespace std;

Enemy::Enemy()
{
}

Enemy::Enemy(Rect& rect, Map& map, int floor)
{
	random_device rng;
	hp = rng() % 5 + floor + 2;
	atk = rng() % 3 + floor;
	def = rng() % 3 + floor;
	minAreaX = rect.left;
	maxAreaX = rect.right;
	minAreaY = rect.top;
	maxAreaY = rect.bottom;

	do {
		x = rng() % rect.width + rect.left;
		y = rng() % rect.height + rect.top;
	} while (map.Check(x, y) != ' ' || map.Check(x, y) == '<');
}

Enemy::~Enemy()
{
}

void Enemy::Move(Map& map)
{
	int dirX, dirY;
	random_device rng;
	do {
		dirX = 0;
		dirY = 0;
		switch (rng() % 5) {
		case 0:
			dirY++;
			break;
		case 1:
			dirX--;
			break;
		case 2:
			dirX++;
			break;
		case 3:
			dirY--;
			break;
		case 4:
			map.Add(x, y, 'E');
			return;
		}
	} while (map.Check(x + dirX, y + dirY) != ' ' 
		|| (x + dirX < minAreaX || x + dirX > maxAreaX 
		|| y + dirY < minAreaY || y + dirY > maxAreaY));

	x += dirX;
	y += dirY;
	map.Add(x, y, 'E');
}

int Enemy::Damage(int dmg)
{
	random_device rng;
	int reduce = max(dmg - def, (int)rng() % 2);
	hp -= reduce;
	return reduce;
}

bool Enemy::Battle(int px, int py, Map& map)
{
	if (px == x || py == y) {
		map.Add(x, y, 'E');
		return true;
	}
	else {
		int dx = x - px;
		int dy = y - py;
		if (map.Check((x + dx), y) == ' ') {
			x += dx;
			map.Add(x, y, 'E');
		}
		else if (map.Check(x, (y + dy)) == ' ') {
			y += dy;
			map.Add(x, y, 'E');
		}
		else {
			map.Add(x, y, 'E');
		}
		return false;
	}
}

string Enemy::StatusDisplay()
{
	stringstream ss;
	ss << "Enemy HP:" << hp
		<< ", ATK:" << atk
		<< ", DEF:" << def << endl;
	return ss.str();
}