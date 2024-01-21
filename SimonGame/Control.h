#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include "Button.h"

#define START 0		//게임 시작 초기 상태
#define PLAY 1		//게임 시작 상태
#define PRESS 2		//플레이어가 버튼을 눌렀을 때 상태
#define PATTERN 3	//눌러야 할 패턴을 보여주는 단계
#define END 4		//게임 종료 상태

using namespace std;

class Control
{
private:
	Sound sound1, sound2; 
	SoundBuffer buffer1, buffer2;
	vector<Button> Buttons;			//버튼 벡터
	vector<int> pattern;			//패턴이 저장된 벡터
	int cur;						//현재 눌러야 하는 버튼을 가리키는 변수
	int level;						//단계
	int size;						//버튼의 갯수
	int delay;						//다음 패턴까지의 딜레이
	int state;						//현재 상태
	int lightOn;					//불이 켜진 버튼을 가리키는 변수
public:
	Control();
	void button_add();					//버튼 추가
	void draw(RenderWindow& window);	//화면에 그리기
	void levelUp();
	void isClicked(Vector2i& pos);		//어떤 버튼이 눌렸는지
	void update();						//상태 업데이트
	void setState(int _state);			//현재 상태를 지정
	int getState();						//현재 상태를 반환
	int getLevel();						//현재 단계를 반환
	void restart();						//재시작
};