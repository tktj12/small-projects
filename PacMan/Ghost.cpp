#include "Ghost.h"

Ghost::Ghost()
{
	level = 1;
	check_frame = 0;
	speed = 0.8f;
	direction = UP;
}
void Ghost::init(const char* file_name)
{
	texture.loadFromFile(file_name);
	body.setTexture(texture);
	body.setTextureRect(IntRect(0, 0, 40, 40));
}
void Ghost::search(Stage& stage)
{
	Vector2f pos = body.getPosition();
	int cnt = 0;
	array<int, 3> way;

	if (direction != DOWN && !stage.isBlocked(Vector2f(pos.x, pos.y - 10.f)))
		way[cnt++] = UP;
	if (direction != LEFT && !stage.isBlocked(Vector2f(pos.x + 10.f, pos.y)))
		way[cnt++] = RIGHT;
	if (direction != UP && !stage.isBlocked(Vector2f(pos.x, pos.y + 10.f)))
		way[cnt++] = DOWN;
	if (direction != RIGHT && !stage.isBlocked(Vector2f(pos.x - 10.f, pos.y)))
		way[cnt++] = LEFT;

	if (cnt) {
		srand(clock() * rand());
		direction = way[rand() % cnt];
	}
	else {
		if (direction == LEFT)
			direction = RIGHT;
		else if (direction == RIGHT)
			direction = LEFT;
	}
}
void Ghost::levelUp()
{
	level++;
	speed += 0.2f;
}
void Ghost::move(Stage& stage)
{
	Vector2f pos = body.getPosition();
	if (pos.x < -40.f)
		pos = Vector2f(950.f, 505.f);
	else if (pos.x > 950.f)
		pos = Vector2f(-40.f, 505.f);

	Vector2f pre = pos;
	if (direction == UP) {
		pos.y -= speed;
		if ((int)pre.y % 50 > 5 && (int)pos.y % 50 <= 5)
			search(stage);
	}
	else if (direction == RIGHT) {
		pos.x += speed;
		if ((int)pre.x % 50 < 5 && (int)pos.x % 50 >= 5)
			search(stage);
	}
	else if (direction == DOWN) {
		pos.y += speed;
		if ((int)pre.y % 50 < 5 && (int)pos.y % 50 >= 5)
			search(stage);
	}
	else if (direction == LEFT) {
		pos.x -= speed;
		if ((int)pre.x % 50 > 5 && (int)pos.x % 50 <= 5)
			search(stage);
	}

	body.setPosition(pos);

	check_frame++;
	if (check_frame > 20 * 200) {
		levelUp();
		check_frame = 0;
	}
}
bool Ghost::intersects(const FloatRect& pacman)
{
	if (body.getGlobalBounds().intersects(pacman))
		return 1;
	else
		return 0;
}
void Ghost::setPos(float x, float y)
{
	body.setPosition(x, y);
}
Vector2f Ghost::getPos()
{
	return body.getPosition();
}
void Ghost::draw(RenderWindow& window)
{
	window.draw(body);
}