#include "pch.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "Item.h"
#include "Division.h"
#include "Rect.h"

#include <iostream>
#include <conio.h>
#include <algorithm>
#include <string>
#include <vector>
#include <random>
#include <stdlib.h>
#include <chrono>
#include <Windows.h>
#include <sstream>

#define ESC 27
#define ENTER 13

using namespace std;

COORD MoveCursor(char);
COORD MoveCursor(char, int, int);
COORD GetCursorPos();
int GetHurtEnemyIndex(pair<int, int>, vector<Enemy>&);
Item PickedItem(pair<int, int>, vector<Item>&);
void Display(Map&, list<string>&, int);

int main()
{
	char input;
	random_device rng;
	Player player;
	bool isClearFloor;
	int floorNumber = 0;
	list<string> hud;

	while (true) {
		floorNumber++;
		isClearFloor = false;
		vector<Enemy> enemyList;
		vector<Item> itemList;
		Map map(floorNumber);

		map.Divide();
		map.CreatePath();
		map.CreateBitMap();
		map.CreateMap();

		int spawnRoomIndex = rng() % map.divList.size();
		player.Spawn(map.divList[spawnRoomIndex].room);

		int goalRoomIndex;
		do {
			goalRoomIndex = rng() % map.divList.size();
		} while (goalRoomIndex == spawnRoomIndex);
		Rect goalRoom = map.divList[goalRoomIndex].room;
		map.Add(goalRoom.width / 2 + goalRoom.left,
			goalRoom.height / 2 + goalRoom.top, '%');

		for (int i = 0; i < map.divList.size(); i++) {
			Rect room = map.divList[i].room;
			int enemyNum = rng() % (room.measure / 100 + 1) + 
									min(room.width, room.height) / 10 + 
									(room.measure / 40);
			int itemNum = rng() % (max(room.width, room.height) / 10 + 1) + 1;
			for (int j = 0; j < enemyNum; j++) {
				enemyList.push_back(Enemy(room, map, floorNumber));
			}
			for (int j = 0; j < itemNum; j++) {
				itemList.push_back(Item(room, map, floorNumber));
			}
		}

		map.Add(player.x, player.y, '@');
		for (int i = 0; i < enemyList.size(); i++) {
			map.Add(enemyList[i].x, enemyList[i].y, ' ');
			map.Add(enemyList[i].x, enemyList[i].y, 'E');
		}
		hud.push_back(player.StatusDisplay());
		Display(map, hud, floorNumber);


		do {
			hud.clear();
			input = _getch();

			if (input == 'q') {
				return 0;
			}
			else if (input == 'c') {
				isClearFloor = true;
			}
			//索敵
			else if (input == 'f') {
				COORD cursorPos, messagePos;
				HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
				CONSOLE_SCREEN_BUFFER_INFO info;

				cursorPos.X = player.x;
				cursorPos.Y = player.y;
				GetConsoleScreenBufferInfo(handle, &info);
				messagePos = info.dwCursorPosition;

				cout << "調べたい敵にカーソルを合わせてください(Escで終了)" << endl;
				do {
					stringstream ss;
					ss << "調べたい敵にカーソルを合わせてください(Escで終了)" << endl;
					hud.push_back(ss.str());
					SetConsoleCursorPosition(handle, cursorPos);

					input = _getch();
					
					switch (input) {
					case 's':
						cursorPos.Y++;
						break;
					case 'a':
						cursorPos.X--;
						break;
					case 'd':
						cursorPos.X++;
						break;
					case 'w':
						cursorPos.Y--;
						break;
					}
					SetConsoleCursorPosition(handle, cursorPos);
					Enemy foundEnemy;
					switch (map.GetMap()[cursorPos.Y][cursorPos.X]) {
					case 'E':
						for (int i = 0; i < enemyList.size(); i++) {
							if (enemyList[i].x == cursorPos.X && enemyList[i].y == cursorPos.Y) {
								foundEnemy = enemyList[i];
							}
						}
						hud.push_back(foundEnemy.StatusDisplay());
						break;
					}
					hud.push_front(player.StatusDisplay());
					Display(map, hud, floorNumber);
				} while (input != ESC);
				hud.push_front(player.StatusDisplay());
				Display(map, hud, floorNumber);
			}
			//メニュー(Enter)
			else if (input == 'e') {
				do {
					cout << "使用/装備するアイテムを選択してください(Spaceで選択、Escで終了)" << endl;
					cout << "持ち物" << endl;
					COORD defaultCursorPos = GetCursorPos();
					for (int i = 0; i < player.belongings.size(); i++) {
						Item & displayItem = player.belongings[i];
						switch (displayItem.type) {
						case 0:
							cout << "食料(HP:+" << displayItem.hp
								<< ", HNG:+" << displayItem.hng << ")" << endl;
							break;
						case 1:
							cout << "武器(ATK:+" << displayItem.atk
								<< ") " << displayItem.isEquiped << endl;
							break;
						case 2:
							cout << "防具(DEF:+" << displayItem.def
								<< ") " << displayItem.isEquiped << endl;
							break;
						}
					}
					do {
						input = _getch();
						MoveCursor(input);
						if (input == ' ') {
							int selectedPos = GetCursorPos().Y - defaultCursorPos.Y;
							if (selectedPos >= 0 && selectedPos < player.belongings.size()) {
								Item &usingItem = player.belongings[selectedPos];
								switch (usingItem.type) {
								case 0:
								{
									int recoverHP = min(player.maxHP - player.hp, usingItem.hp);
									int recoverHng = min(player.maxHng - player.hng, usingItem.hng);
									player.hp += max(recoverHP, 0);
									player.hng += max(recoverHng, 0);
									if (usingItem.hp == recoverHP || usingItem.hng == recoverHng) {
										if (player.weaponIndex > selectedPos) player.weaponIndex--;
										if (player.armorIndex > selectedPos) player.armorIndex--;
										player.belongings.erase(player.belongings.begin() + selectedPos);
									}
									else {
										usingItem.hp -= recoverHP;
										usingItem.hng -= recoverHng;
									}
									break;
								}
								case 1:
									player.Equip(usingItem, selectedPos);
									player.CalcAtk();
									break;
								case 2:
									player.Equip(usingItem, selectedPos);
									player.CalcDef();
									break;
								}
							}
						}
					} while (input != ESC && input != ' ');
					hud.push_back(player.StatusDisplay());
					Display(map, hud, floorNumber);
				} while (input != ESC);
			}
			//捨てる
			else if (input == 'r') {
				do{
					cout << "捨てるアイテムを選択してください(Spaceで選択、Escで終了)" << endl;
					cout << "持ち物" << endl;
					COORD defaultCursorPos = GetCursorPos();
					for (int i = 0; i < player.belongings.size(); i++) {
						Item & displayItem = player.belongings[i];
						switch (displayItem.type) {
						case 0:
							cout << "食料(HP:+" << displayItem.hp
								<< ", HNG:+" << displayItem.hng << ")" << endl;
							break;
						case 1:
							cout << "武器(ATK:+" << displayItem.atk
								<< ") " << displayItem.isEquiped << endl;
							break;
						case 2:
							cout << "防具(DEF:+" << displayItem.def
								<< ") " << displayItem.isEquiped << endl;
							break;
						}
					}
					do {
						input = _getch();
						MoveCursor(input);
						if (input == ' ') {
							int selectedPos = GetCursorPos().Y - defaultCursorPos.Y;
							if (selectedPos >= 0 && selectedPos < player.belongings.size()) {
								if (selectedPos == player.weaponIndex) {
									player.equipWeapon = Item();
									player.isEquipWeapon = false;
									player.CalcAtk();
									player.belongings[selectedPos].isEquiped.clear();
								}
								else if (selectedPos == player.armorIndex) {
									player.equipArmor = Item();
									player.isEquipArmor = false;
									player.CalcDef();
									player.belongings[selectedPos].isEquiped.clear();
								}
								player.belongings.erase(player.belongings.begin() + selectedPos);
								if (player.weaponIndex > selectedPos) player.weaponIndex--;
								if (player.armorIndex > selectedPos) player.armorIndex--;
							}
						}
					} while (input != ESC && input != ' ');
					hud.push_back(player.StatusDisplay());
					Display(map, hud, floorNumber);
				} while (input != ESC);
			}
			//行動
			else {
				map.Add(player.x, player.y, ' ');
				pair<int, int> direction = player.Move(input);
				switch (map.MapCheck(direction)) {
				case 0:
					player.SetPosition(direction);
					break;
				case 1:
					break;
				case 2:
				{
					Enemy &hurtEnemy = enemyList[GetHurtEnemyIndex(direction, enemyList)];
					int damage = hurtEnemy.Damage(player.atk);
					stringstream ss;
					ss << "Playerの攻撃、Enemyに" << damage << "ダメージ\n";
					hud.push_back(ss.str());
					break;
				}
				case 3:
					isClearFloor = true;
				case 4:
					player.Pick(PickedItem(direction, itemList));
					break;
				}
				map.Add(player.x, player.y, '@');

				for (int i = 0; i < itemList.size(); i++) {
					if (itemList[i].isPicked) {
						map.Add(itemList[i].x, itemList[i].y, ' ');
						itemList.erase(itemList.begin() + i);
					}
					else {
						itemList[i].Register(map);
					}
				}

				for (int i = 0; i < enemyList.size(); i++) {
					map.Add(enemyList[i].x, enemyList[i].y, ' ');
					if (enemyList[i].hp <= 0) {
						enemyList.erase(enemyList.begin() + i);
						stringstream ss;
						int exp = max(floorNumber - player.level, 1);
						int isLevelUp = player.SumEXP(exp);
						ss << "Enemyを倒した、" << exp << "EXPを獲得" << endl;
						while (isLevelUp >= 0) {
							ss << "レベルアップ！ ";
							switch (isLevelUp) {
							case 0:
								ss << "最大HPが1上がった" << endl;
								break;
							case 1:
								ss << "基礎ATKが1上がった" << endl;
								break;
							case 2:
								ss << "基礎DEFが1上がった" << endl;
								break;
							case 3:
								ss << "最大HNGが1上がった" << endl;
								break;
							}
							isLevelUp = player.SumEXP(0);
						}
						hud.push_back(ss.str());
					}
					else if (abs(enemyList[i].x - player.x) <= 1 && abs(enemyList[i].y - player.y) <= 1) {
						if (enemyList[i].Battle(player.x, player.y, map)) {
							hud.push_front(enemyList[i].StatusDisplay());
							int damage = player.Damage(enemyList[i].atk);
							stringstream ss;
							ss << "Enemyの攻撃、Playerに" << damage << "ダメージ\n";
							hud.push_back(ss.str());
						}
					}
					else {
						enemyList[i].Move(map);
					}
				}
				hud.push_front(player.StatusDisplay());
				Display(map, hud, floorNumber);
				if (player.hp <= 0 || player.hng <= 0) {
					cout << "Game Over" << endl;
					Sleep(1000);
					return 0;
				}

			}
		} while (!isClearFloor);
	}

	return 0;

}

COORD MoveCursor(char input)
{
	COORD cursorPos = GetCursorPos();
	return MoveCursor(input, cursorPos.X, cursorPos.Y);
}

COORD MoveCursor(char input, int x, int y)
{
	COORD cursorPos;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

	cursorPos.X = x;
	cursorPos.Y = y;

	SetConsoleCursorPosition(handle, cursorPos);

	switch (input) {
	case 's':
		cursorPos.Y++;
		break;
	case 'a':
		cursorPos.X--;
		break;
	case 'd':
		cursorPos.X++;
		break;
	case 'w':
		cursorPos.Y--;
		break;
	}
	SetConsoleCursorPosition(handle, cursorPos);

	return cursorPos;
}

COORD GetCursorPos()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;

	GetConsoleScreenBufferInfo(handle, &info);
	return info.dwCursorPosition;
}

int GetHurtEnemyIndex(pair<int, int> position, vector<Enemy> &enemyList)
{
	for (int i = 0; i < enemyList.size(); i++) {
		if (enemyList[i].x == position.first && enemyList[i].y == position.second) {
			return i;
		}
	}
	return -1;
}

Item PickedItem(pair<int, int> position, vector<Item> &itemList)
{
	for (int i = 0; i < itemList.size(); i++) {
		if (itemList[i].x == position.first && itemList[i].y == position.second) {
			itemList[i].isPicked = true;
			return itemList[i];
		}
	}
	return -1;
}

void Display(Map& map, list<string>& hud, int floor)
{
	system("cls");
	map.Display();
	cout << "B" << floor << "階" << endl;
	for (string s : hud) {
		cout << s;
	}
	hud.clear();
}