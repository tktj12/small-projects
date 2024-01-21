#pragma once
#include "Ghost.h"

class GhostSet
{
private:
	array<Ghost, 4> ghosts;
public:
	GhostSet();
	void init();
	void update(Stage& stage);
	int collision(FloatRect pacman);
	void draw(RenderWindow& window);
	int getLevel();
};