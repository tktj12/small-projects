#pragma once
#include "Point.h"
#include <array>

using namespace std;

class Stage
{
private:
	array<string, 22> map;
	vector<Sprite> walls;
	vector<Point> points;
	CircleShape special;
	Texture wall_texture;
	int flag_special;
public:
	Stage();
	void init();
	int isBlocked(const Vector2f& pos);
	int isBlocked(const FloatRect& rect);
	int isGetPoint(const FloatRect& pacman);
	void draw(RenderWindow& window);
};