#pragma once
#include <SFML/Audio.hpp>
#include "GhostSet.h"

#define START 0
#define PLAY 1
#define GAMEOVER 2

class Pacman
{
private:
	array<SoundBuffer, 4> buffer;
	array<Sound, 4> sound;
	array<Texture, 4> texture;
	array<Texture, 4> mode;
	Sprite body;
	int frame_texture;
	int frame_mode;
	int idx;
public:
	int nStage;
	Pacman();
	void init();
	void move(Stage& stage);
	void update(Stage& stage, GhostSet& ghostSet, int& score, int& state);
	void draw(RenderWindow& window);
	void playStartSound();
};