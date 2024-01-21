#include "Button.h"

Button::Button()
{
	r = g = b = 0;
	body.setRadius(74.f);
}

void Button::setColor(int _r, int _g, int _b)
{
	r = _r; g = _g; b = _b;
	body.setFillColor(Color(r, g, b, 100));
}

void Button::setPos(float x, float y)
{
	body.setPosition(Vector2f(x, y));
}

void Button::draw(RenderWindow& window)
{
	window.draw(body);
}

Vector2f Button::getPos()
{
	return body.getPosition();
}

void Button::light()
{
	body.setFillColor(Color(r, g, b, 255));
}

void Button::off()
{
	body.setFillColor(Color(r, g, b, 100));
}