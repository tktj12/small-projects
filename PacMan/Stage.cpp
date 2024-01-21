#include "Stage.h"

Stage::Stage()
{
	Point point;

	wall_texture.loadFromFile("Images/wall.png");
	Sprite wall_sprite;
	wall_sprite.setTexture(wall_texture);
	wall_sprite.setTextureRect(IntRect(0, 0, 50, 50));

	special.setFillColor(Color::Green);
	special.setRadius(10.f);
	special.setPosition(465.f, 215.f);
	flag_special = 1;

	array<string, 22> rmaps{
			"###################",
			"#........#........#",
			"#.##.###.#.###.##.#",
			"#.##.###.#.###.##.#",
			"#........o........#",
			"#.##.#.#####.#.##.#",
			"#....#...#...#....#",
			"####.### # ###.####",
			"   #.#       #.#   ",
			"####.# ## ## #.####",
			"    .  #   #  .    ",
			"####.# ##### #.####",
			"   #.#       #.#   ",
			"####.# ##### #.####",
			"#........#........#",
			"#.##.###.#.###.##.#",
			"#..#.....p.....#..#",
			"##.#.#.#####.#.#.##",
			"#....#...#...#....#",
			"#.######.#.######.#",
			"#.................#",
			"###################" };
	map = rmaps;

	for (int i = 0; i < 22; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			if (map[i][j] == '#') {
				wall_sprite.setPosition(j * 50, i * 50);
				walls.push_back(wall_sprite);
			}
			else if (map[i][j] == '.') {
				point.setPos(j * 50 + 20, i * 50 + 20);
				points.push_back(point);
			}
		}
	}
}
void Stage::init()
{
	for (auto& e : points)
	{
		e.flag = 1;
	}
	flag_special = 1;
}
int Stage::isBlocked(const Vector2f& pos)
{
	FloatRect tp(pos, Vector2f(40.f, 40.f));
	for (auto& e : walls)
	{
		if (e.getGlobalBounds().intersects(tp))
			return 1;
	}
	return 0;
}
int Stage::isBlocked(const FloatRect& rect)
{
	for (auto& e : walls)
	{
		if (e.getGlobalBounds().intersects(rect))
			return 1;
	}
	return 0;
}
int Stage::isGetPoint(const FloatRect& pacman)
{
	for (auto& e : points)
	{
		if (e.rect.getGlobalBounds().intersects(pacman) && e.flag) {
			e.flag = 0;
			return 1;
		}
	}
	if (special.getGlobalBounds().intersects(pacman) && flag_special) {
		flag_special = 0;
		return 2;
	}

	return 0;
}
void Stage::draw(RenderWindow& window)
{
	for (auto& e : walls)
	{
		window.draw(e);
	}
	for (auto& e : points)
	{
		if (e.flag)
			window.draw(e.rect);
	}
	if (flag_special)
		window.draw(special);
}