#pragma once
#include "Stage.h"

#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

class Ghost
{
private:
	Texture texture;
	Sprite body;
	int check_frame;
public:
	float speed;
	int direction;
	int level;
	Ghost();
	void init(const char* file_name);
	void search(Stage& stage);
	void levelUp();
	void move(Stage& stage);
	bool intersects(const FloatRect& pacman);
	void setPos(float x, float y);
	Vector2f getPos();
	void draw(RenderWindow& window);
};