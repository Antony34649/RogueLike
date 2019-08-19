#include "pch.h"
#include "Player.h"

using namespace std;

Player::Player()
{
	equipWeapon = Item();
	equipArmor = Item();
	isEquipWeapon = false;
	isEquipArmor = false;

	baseAtk = 1;
	baseDef = 1;
	maxHP = 10;
	maxHng = 20;
	level = 1;
	exp = 0;

	hp = maxHP;
	CalcAtk();
	CalcDef();
	hng = maxHng;
	amountMove = 0;
	belongings.push_back(Item(0));
}

Player::~Player()
{
}

string Player::StatusDisplay()
{
	string escHP, escHng;
	if (hp <= 3) {
		escHP = "\x1b[31m";
	}
	if (hng <= 3) {
		escHng = "\x1b[31m";
	}
	stringstream ss;
	ss  << "Player HP:" << escHP << hp << "\x1b[39m" << "/" << maxHP
		<< " ATK:" << atk
		<< " DEF:" << def
		<< " HNG:" << escHng << hng  << "\x1b[39m" << "/" << maxHng 
		<< " LV:" << level 
		<< " EXP:" << exp << "/" << pow(level,2) << endl;
	return ss.str();
}

pair<int,int> Player::Move(char input)
{
	int dirX = 0, dirY = 0;
	switch (input) {
	case 's':
		dirY++;
		break;
	case 'a':
		dirX--;
		break;
	case 'd':
		dirX++;
		break;
	case 'w':
		dirY--;
		break;
	}
	amountMove++;
	if (amountMove >= 10) {
		amountMove = 0;
		hng--;
	}
	return pair<int, int>(x + dirX, y + dirY);
}

void Player::Pick(Item item)
{
	if (item.type != -1) {
		belongings.push_back(item);
	}
}

int Player::Damage(int dmg)
{
	random_device rng;
	int reduce = max(dmg - def, (int)(rng()%2));
	hp -= reduce;
	return reduce;
}

void Player::SetPosition(pair<int, int> position)
{
	x = position.first;
	y = position.second;
}

void Player::Equip(Item &equipItem, int index) {
	switch (equipItem.type) {
	case 1:
		if (isEquipWeapon) {
			belongings[weaponIndex].isEquiped.clear();
		}
		else {
			isEquipWeapon = true;
		}
		equipWeapon = equipItem;
		weaponIndex = index;
		belongings[index].isEquiped = " ‘•”õ’†";
		break;
	case 2:
		if (isEquipArmor) {
			belongings[armorIndex].isEquiped.clear();
		}
		else {
			isEquipArmor = true;
		}
		equipArmor = equipItem;
		armorIndex = index;
		belongings[index].isEquiped = " ‘•”õ’†";
		break;
	}
}

void Player::Spawn(Rect& rect)
{
	x = rect.width / 2 + rect.left;
	y = rect.height / 2 + rect.top;
}

int Player::SumEXP(int e)
{
	exp += e;
	if (exp >= pow(level,2)) {
		return LevelUp();
	}
	else {
		return -1;
	}
}

int Player::LevelUp()
{
	exp -= (int)pow(level,2);
	level++;
	random_device rng;
	int riseStatus = rng() % 4;
	switch (riseStatus) {
	case 0:
		if (hp == maxHP) {
			hp++;
		}
		maxHP++;
		break;
	case 1:
		baseAtk++;
		CalcAtk();
		break;
	case 2:
		baseDef++;
		CalcDef();
		break;
	case 3:
		if (hng == maxHng) {
			hng++;
		}
		maxHng++;
		break;
	}
	return riseStatus;
}