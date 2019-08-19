#pragma once

#include "Rect.h"
#include "Item.h"
#include <string>
#include <vector>
#include <random>
#include <sstream>
#include <algorithm>
#include <iostream>

class Player
{
public:
	int x, y, hp, atk, def, hng, amountMove;
	int maxHP, baseAtk, baseDef, maxHng, level, exp;
	std::vector<Item> belongings;
	Item equipWeapon, equipArmor;
	int weaponIndex, armorIndex;
	bool isEquipWeapon, isEquipArmor;

	Player();
	~Player();

	std::string StatusDisplay();
	std::pair<int,int> Move(char);
	int Damage(int dmg);
	void Pick(Item);
	void SetPosition(std::pair<int, int>);
	void Equip(Item&, int);
	void CalcAtk() { atk = baseAtk + equipWeapon.atk; }
	void CalcDef() { def = baseDef + equipArmor.def; }
	void Spawn(Rect&);
	int SumEXP(int);
	int LevelUp();
};

