#include "Pacman.h"

Pacman::Pacman()
{
	idx = frame_mode = frame_texture = 0;
	nStage = 1;

	buffer[0].loadFromFile("start.wav");
	buffer[1].loadFromFile("eating.wav");
	buffer[2].loadFromFile("special.wav");
	buffer[3].loadFromFile("dead.wav");

	for (int i = 0; i < 4; i++)
		sound[i].setBuffer(buffer[i]);

	texture[0].loadFromFile("Images/pacman1.png");
	texture[1].loadFromFile("Images/pacman2.png");
	texture[2].loadFromFile("Images/pacman3.png");
	texture[3].loadFromFile("Images/pacman2.png");

	mode[0].loadFromFile("Images/mode1.png");
	mode[1].loadFromFile("Images/mode2.png");
	mode[2].loadFromFile("Images/mode3.png");
	mode[3].loadFromFile("Images/mode2.png");

	body.setTexture(texture[1]);
	body.setTextureRect(IntRect(0, 0, 40, 40));
	init();
}
void Pacman::init()
{
	body.setPosition(455.f, 805.f);
}
void Pacman::move(Stage& stage)
{
	if (body.getGlobalBounds().left < -40.f) {
		body.move(990.f, 0);
	}
	else if (body.getGlobalBounds().left > 950.f) {
		body.move(-990.f, 0);
	}

	if (Keyboard::isKeyPressed(Keyboard::Right)) {
		body.setRotation(0.f);
		body.setOrigin(0.f, 0.f);
		body.move(1.f, 0.f);
		if (stage.isBlocked(body.getGlobalBounds()))
			body.move(-1.f, 0.f);
	}
	if (Keyboard::isKeyPressed(Keyboard::Left)) {
		body.setRotation(180.f);
		body.setOrigin(40.f, 40.f);
		body.move(-1.f, 0.f);
		if (stage.isBlocked(body.getGlobalBounds()))
			body.move(1.f, 0.f);
	}
	if (Keyboard::isKeyPressed(Keyboard::Up)) {
		body.setRotation(270.f);
		body.setOrigin(40.f, 0.f);
		body.move(0.f, -1.f);
		if (stage.isBlocked(body.getGlobalBounds()))
			body.move(0.f, 1.01f);
	}
	if (Keyboard::isKeyPressed(Keyboard::Down)) {
		body.setRotation(90.f);
		body.setOrigin(0.f, 40.f);
		body.move(0.f, 1.f);
		if (stage.isBlocked(body.getGlobalBounds()))
			body.move(0.f, -1.f);
	}
}
void Pacman::update(Stage& stage, GhostSet& ghostSet, int& score, int& state)
{
	move(stage);

	if (frame_mode < 1) {
		if (ghostSet.collision(body.getGlobalBounds())) {
			sound[3].play();
			init();
			ghostSet.init();
			stage.init();
			nStage = 1;
			state = GAMEOVER;
		}
	}

	int tp = stage.isGetPoint(body.getGlobalBounds());
	if (tp) {
		score++;
		if (tp == 1)
			sound[1].play();
		else if (tp == 2) {
			sound[2].play();
			frame_mode = 1000;
		}

		if (score % 157 == 0) {
			score++;
			stage.init();
			ghostSet.init();
			init();
			nStage++;
			sound[0].play();
		}
	}

	if (frame_mode > 0) frame_mode--;
	if (frame_texture > 0) frame_texture--;
	else {
		frame_texture = 40;
		idx = (idx + 1) % 4;
		if (frame_mode > 0)
			body.setTexture(mode[idx]);
		else
			body.setTexture(texture[idx]);
	}
}
void Pacman::draw(RenderWindow& window)
{
	window.draw(body);
}
void Pacman::playStartSound()
{
	sound[0].play();
}