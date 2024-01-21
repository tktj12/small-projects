#include "point.h"

Point::Point()
{
	flag = 1;
	rect.setSize(Vector2f(10.f, 10.f));
	rect.setFillColor(Color::White);
}
void Point::setPos(int x, int y)
{
	rect.setPosition((float)x, (float)y);
}
Vector2f Point::getPos()
{
	return rect.getPosition();
}