#pragma once
#include <SFML/Audio.hpp>
#include <vector>
#include "Button.h"

#define START 0		//���� ���� �ʱ� ����
#define PLAY 1		//���� ���� ����
#define PRESS 2		//�÷��̾ ��ư�� ������ �� ����
#define PATTERN 3	//������ �� ������ �����ִ� �ܰ�
#define END 4		//���� ���� ����

using namespace std;

class Control
{
private:
	Sound sound1, sound2; 
	SoundBuffer buffer1, buffer2;
	vector<Button> Buttons;			//��ư ����
	vector<int> pattern;			//������ ����� ����
	int cur;						//���� ������ �ϴ� ��ư�� ����Ű�� ����
	int level;						//�ܰ�
	int size;						//��ư�� ����
	int delay;						//���� ���ϱ����� ������
	int state;						//���� ����
	int lightOn;					//���� ���� ��ư�� ����Ű�� ����
public:
	Control();
	void button_add();					//��ư �߰�
	void draw(RenderWindow& window);	//ȭ�鿡 �׸���
	void levelUp();
	void isClicked(Vector2i& pos);		//� ��ư�� ���ȴ���
	void update();						//���� ������Ʈ
	void setState(int _state);			//���� ���¸� ����
	int getState();						//���� ���¸� ��ȯ
	int getLevel();						//���� �ܰ踦 ��ȯ
	void restart();						//�����
};