#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Point
{
public:
	RectangleShape rect;
	int flag;
	Point();
	void setPos(int x, int y);
	Vector2f getPos();
};