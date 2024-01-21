#include <string>
#include "Control.h"

void textSet(Text& text1, Text& text2, Control& cont, int& but_num);

int main()
{
	RenderWindow window(VideoMode(450, 600), "Simon Game");
	window.setFramerateLimit(120);

	Font font;
	if (!font.loadFromFile("arial.ttf")) {
		exit(1);
	}
	Text text1, text2;
	text1.setFont(font);				text2.setFont(font);
	text1.setFillColor(Color::White);	text2.setFillColor(Color::White);
	text1.setCharacterSize(20);			text2.setCharacterSize(20);

	Control cont;
	Event event;
	int but_num(4);
	Vector2i pos;
	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
				window.close();
				break;
			case Event::MouseButtonPressed:
				if (event.key.code == Mouse::Left) {
					//state º° ½ÇÇà
					switch (cont.getState())
					{
					case START:
						for (int i = 0; i < but_num; i++)
							cont.button_add();
						cont.levelUp();
						cont.setState(PATTERN);
						break;
					case PLAY:
						pos = Mouse::getPosition(window);
						cont.isClicked(pos);
						break;
					case END:
						cont.restart();
						break;
					}
				}
				else if (event.key.code == Mouse::Right){
					but_num++;
					if (but_num > 10)
						but_num = 4;
				}
				break;
			}
		}
		textSet(text1, text2, cont, but_num);
		cont.update();

		window.clear();
		cont.draw(window);
		window.draw(text1); window.draw(text2);
		window.display();
	}
}

void textSet(Text& text1, Text& text2, Control& cont, int& but_num)
{
	switch (cont.getState())
	{
	case START:
		text1.setPosition(30.0f, 200.0f);		text2.setPosition(230.0f, 270.0f);
		text1.setString("Mouse right click to set number of button\n\n\nNumber of button : \n\n\nMouse left click to start");
		text2.setString(to_string(but_num));
		break;
	case END:
		text1.setPosition(100.0f, 250.0f);		text2.setPosition(250.0f, 250.0f);
		text1.setString("Your score : \n\n\nMouse left click to restart");
		text2.setString(to_string(cont.getLevel()));
		break;
	default:
		text1.setPosition(30.0f, 530.0f);		text2.setPosition(100.0f, 530.0f);
		text1.setString("Level : ");
		text2.setString(to_string(cont.getLevel()));
		break;
	}
}