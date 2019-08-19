#include "pch.h"
#include "Division.h"

#define MIN_ROOM_SIDE 3

using namespace std;

Division::Division()
{
	outer = Rect();
	room = Rect();
}

Division::Division(int t, int b, int l, int r)
{
	outer = Rect(t,b,l,r);
	CreateRoom();
}

Division::~Division()
{
}

void Division::CreateRoom()
{
	random_device rng;
	int roomWidth = rng() % ((outer.width - 3) - MIN_ROOM_SIDE) + MIN_ROOM_SIDE;
	int roomHeight = rng() % ((outer.height - 3) - MIN_ROOM_SIDE) + MIN_ROOM_SIDE;

	int roomLeftSide = rng() % (outer.width - roomWidth - 3) + outer.left + 2;
	int roomTopSide = rng() % (outer.height - roomHeight - 3) + outer.top + 2;

	room = Rect(pair<int, int>(roomLeftSide, roomTopSide), roomWidth, roomHeight);
}

void Division::CreatePath()
{
	int maxPath = 4;
	if (outer.top == 0) {
		maxPath--;
	}
}
