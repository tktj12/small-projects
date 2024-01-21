#include "GhostSet.h"

GhostSet::GhostSet()
{
	ghosts[0].init("Images/red_right_1.png");
	ghosts[1].init("Images/blue_right_1.png");
	ghosts[2].init("Images/pink_right_1.png");
	ghosts[3].init("Images/yellow_right_1.png");
	init();
}
void GhostSet::init()
{
	ghosts[0].setPos(455.f, 509.f);
	ghosts[1].setPos(405.f, 509.f);
	ghosts[2].setPos(505.f, 509.f);
	ghosts[3].setPos(455.f, 459.f);
	for (auto& e : ghosts) {
		e.direction = UP;
		e.level = 1;
		e.speed = 0.8f;
	}
}
void GhostSet::update(Stage& stage)
{
	for (auto& e : ghosts)
	{
		e.move(stage);
	}
}
int GhostSet::collision(FloatRect pacman)
{
	pacman.left += 15.f;
	pacman.top += 15.f;
	pacman.width = 10;
	pacman.height = 10;

	for (auto& e : ghosts)
	{
		if (e.intersects(pacman))
			return 1;
	}
	return 0;
}
void GhostSet::draw(RenderWindow& window)
{
	for (auto& e : ghosts)
		e.draw(window);
}
int GhostSet::getLevel()
{
	return ghosts[0].level;
}