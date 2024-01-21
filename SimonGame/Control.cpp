#include <iostream>
#include <ctime>
#include "Control.h"

Control::Control()
{
	cur = level = size = lightOn = 0;
	delay = 120;
	state = START;

	if (!buffer1.loadFromFile("click_sound.wav"))
		exit(1);
	if (!buffer2.loadFromFile("pattern_sound.wav"))
		exit(1);

	sound1.setBuffer(buffer1);
	sound2.setBuffer(buffer2);
}

void Control::button_add()
{
	size++;
	Button tp;
	switch (size)
	{
	case 1:
		tp.setColor(255, 0, 0);
		tp.setPos(0.f, 0.f);
		break;
	case 2:
		tp.setColor(0, 0, 255);
		tp.setPos(150.f, 0.f);
		break;
	case 3:
		tp.setColor(255, 0, 221);
		tp.setPos(0.f, 150.f);
		break;
	case 4:
		tp.setColor(255, 187, 0);
		tp.setPos(150.f, 150.f);
		break;
	case 5:
		tp.setColor(29, 219, 22);
		tp.setPos(300.f, 0.f);
		break;
	case 6:
		tp.setColor(255, 94, 0);
		tp.setPos(300.f, 150.f);
		break;
	case 7:
		tp.setColor(0, 216, 255);
		tp.setPos(0.f, 300.f);
		break;
	case 8:
		tp.setColor(0, 84, 255);
		tp.setPos(150.f, 300.f);
		break;
	case 9:
		tp.setColor(255, 228, 0);
		tp.setPos(300.f, 300.f);
		break;
	case 10:
		tp.setColor(0, 255, 0);
		tp.setPos(150.f, 450.f);
		break;
	}
	Buttons.push_back(tp);
}

void Control::draw(RenderWindow& window)
{
	for (auto& e : Buttons)
	{
		e.draw(window);
	}
}

void Control::levelUp()
{
	level++;
	srand(rand() * clock());
	int tp = rand() % size;
	pattern.push_back(tp);
}

void Control::isClicked(Vector2i& pos)
{
	int i = 0;
	Vector2f pos_but, pos_mouse((float)pos.x, (float)pos.y);
	for (auto& e : Buttons)
	{
		pos_but = e.getPos();
		if (pos_mouse.x >= pos_but.x && pos_mouse.x <= pos_but.x + 150 && pos_mouse.y >= pos_but.y && pos_mouse.y <= pos_but.y + 150) {
			state = PRESS;
			e.light();
			delay = 50;
			sound1.play();
			lightOn = i;
			break;
		}
		i++;
	}
}

void Control::update()
{
	if (state == PRESS)
	{
		if (delay) {
			delay--;
		}
		else if (delay == 0) {
			if (lightOn == pattern[cur]) { //맞았을 경우
				Buttons[lightOn].off();
				cur++;
			}
			else { //틀렸을 경우
				Buttons.clear();
				pattern.clear();
				state = END;
				return;
			}

			//패턴을 전부 맞췄을 경우
			if (cur == level) {
				levelUp();
				cur = 0;
				delay = 60;
				state = PATTERN;
			}
			else
				state = PLAY;
		}
	}
	else if (state == PATTERN)
	{
		if (delay) {
			delay--;
			//다음에 눌러야 할 버튼이 동일한 버튼일 경우 구별하기 위해서 불을 미리 끔
			if (delay < 10 && cur > 0)
				Buttons[pattern[cur - 1]].off();
		}
		else {
			if (cur < level) {
				Buttons[pattern[cur]].light();
				cur++;
				sound2.play();

				//단계 별로 딜레이가 줄어듦
				if (level > 16) {
					delay = 40;
				}
				else {
					delay = 120 - level * 5;
				}
			}
			else {
				cur = 0;
				state = PLAY;
			}
		}
	}
}

int Control::getState()
{
	return state;
}

void Control::setState(int _state)
{
	state = _state;
}

void Control::restart()
{
	cur = level = size = 0;
	delay = 120;
	state = START;
}

int Control::getLevel()
{
	return level;
}