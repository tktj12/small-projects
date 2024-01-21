#include "Pacman.h"

#define WIN_SIZE_X 950
#define WIN_SIZE_Y 1150

class TextSet
{
private:
	Font font;
	Text text1, text2, text3, text4;
public:
	TextSet()
	{
		if (!font.loadFromFile("arial.ttf"))
			exit(1);

		text1.setFont(font);				text2.setFont(font);
		text1.setFillColor(Color::White);	text2.setFillColor(Color::White);

		text3.setFont(font);				text4.setFont(font);
		text3.setFillColor(Color::White);	text4.setFillColor(Color::White);
		text3.setCharacterSize(20);			text4.setCharacterSize(20);
		text3.setPosition(240.f, 1100.f);	text4.setPosition(360.f, 1100.f);
	}
	void draw(RenderWindow& window, int state, int score, int level, int stage)
	{
		switch (state)
		{
		case START:
			text1.setCharacterSize(50);
			text1.setPosition(300.f, 500.f);
			text1.setString("Press any key");
			window.draw(text1);
			break;
		case PLAY:
			text1.setCharacterSize(20);
			text1.setPosition(50.f, 1100.f);
			text1.setString("Stage : \t\tScore : \t\tLevel : ");
			text2.setCharacterSize(20);
			text2.setPosition(120.f, 1100.f);
			text2.setString(to_string(stage));
			text3.setString(to_string(score));
			text4.setString(to_string(level));
			window.draw(text1);
			window.draw(text2);
			window.draw(text3);
			window.draw(text4);
			break;
		case GAMEOVER:
			text1.setCharacterSize(50);
			text1.setPosition(100.f, 500.f);
			text1.setString("Your score : \n\nRestart?");
			text2.setCharacterSize(50);
			text2.setPosition(450.f, 500.f);
			text2.setString(to_string(score));
			window.draw(text1);
			window.draw(text2);
			break;
		}
	}
};

int main()
 {
	RenderWindow window(VideoMode(WIN_SIZE_X, WIN_SIZE_Y), "PacMan");
	window.setFramerateLimit(200);

	TextSet text;
	Stage stage;
	Pacman pacman;
	GhostSet ghostSet;

	Event event;
	int score = 1;
	int state = START;
	while (window.isOpen())
	{
		window.pollEvent(event);
		if (event.type == Event::Closed)
			window.close();
		else if (event.type == Event::KeyPressed) {
			if (state == START) {
				state = PLAY;
				pacman.playStartSound();
			}
			else if (state == GAMEOVER)
				state = START;
		}

		window.clear();
		if (state == START)
			score = 1;
		else if (state == PLAY) {
			ghostSet.update(stage);
			pacman.update(stage, ghostSet, score, state);
			stage.draw(window);
			ghostSet.draw(window);
			pacman.draw(window);
		}
		text.draw(window, state, score, ghostSet.getLevel(), pacman.nStage);
		window.display();
	}

	return 0;
}