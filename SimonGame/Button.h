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
	void setColor(int _r, int _g, int _b);	//버튼 색 설정
	void setPos(float x, float y);			//버튼 위치 설정
	void draw(RenderWindow& window);		//화면에 그리기
	Vector2f getPos();						//버튼 위치 반환
	void light();							//버튼 불 켜기
	void off();								//버튼 불 끄기
};