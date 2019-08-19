#pragma once

#include <iostream>

class Rect
{
public:
	int top, bottom, left, right;
	int width, height,measure;

	Rect();
	Rect(int t, int b, int l, int r);
	Rect(std::pair<int, int>, int, int);
	~Rect();

	void Set(int t, int b, int l, int r);
};

