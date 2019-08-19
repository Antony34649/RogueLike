#include "pch.h"
#include "Rect.h"

using namespace std;

Rect::Rect()
{
}

Rect::Rect(int t, int b, int l, int r)
{
	Set(t, b, l, r);
}

Rect::Rect(pair<int, int> position, int w, int h)
{
	int t = position.second;
	int b = position.second + h;
	int l = position.first;
	int r = position.first + w;
	Set(t, b, l, r);
}

Rect::~Rect()
{
}

void Rect::Set(int t, int b, int l, int r)
{
	top = t;
	bottom = b;
	left = l;
	right = r;
	width = r - l;
	height = bottom - top;
	measure = width * height;
}
