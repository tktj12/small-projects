#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Button
{
private:
	CircleShape body;
	int r;
	int g;
	int b;
public:
	Button();
	void setColor(int _r, int _g, int _b);	//��ư �� ����
	void setPos(float x, float y);			//��ư ��ġ ����
	void draw(RenderWindow& window);		//ȭ�鿡 �׸���
	Vector2f getPos();						//��ư ��ġ ��ȯ
	void light();							//��ư �� �ѱ�
	void off();								//��ư �� ����
};