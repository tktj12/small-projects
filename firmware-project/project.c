#ifndef SRC_MAIN_H_
#define SRC_MAIN_H_
#endif

#define GPIO_A 0x40020000u
#define GPIO_B 0x40020400u
#define GPIO_C 0x40020800u
#define GPIO_D 0x40020C00u
#define GPIO_E 0x40021000u
#define GPIO_F 0x40021400u
#define GPIO_G 0x40021800u
#define GPIO_H 0x40021C00u
#define GPIO_I 0x40022000u
#define GPIO_J 0x40022400u
#define GPIO_K 0x40022800u

#define _MODER 0x00u
#define _OSPEEDR 0x08u
#define _PUPDR 0x0Cu
#define _IDR 0x10u
#define _ODR 0x14u
#define _BSRR 0x18u

#define NEXT 1
#define RE 2

#define BTN_NOT_PRESSED 0
#define BTN_PRESSED 	1

#include "bsp.h"

void hwInit(void);
void MyApp();

unsigned int btn_flag1, btn_flag2, switching_flag, state;

int main(void)
{
   hwInit();

   MyApp();
}

void hwInit(void)
{

  bspInit();
  // More Init later
}

void MyDelay(unsigned int n)
{
   volatile unsigned int delay;
   for(delay=0; delay<=n*3600000; delay++);
}

void ClockEnable(unsigned int gpio_port)
{
   *(volatile unsigned int *)(0x40023830u) |= (0x01u)<<(gpio_port-1);
}

int CheckClockStatus(unsigned int gpio_port)
{
   if(*(volatile unsigned int *)(0x40023830u) & (0x01u)<<(gpio_port-1))
      return 1;
   else
      return 0;
}

void LedEnable()
{
   //PG 12
      if(! CheckClockStatus(7)) ClockEnable(7);
      //GPIOG_MODER
      *(volatile unsigned int *)(0x40021800u) &= ~((0b11u)<<(2*12));
      *(volatile unsigned int *)(0x40021800u) |= (0b01u)<<(2*12); //output mode
      //GPIOG_OSPEEDR
      *(volatile unsigned int *)(0x40021808u) |= (0b11u)<<(2*12); //very high
      //GPIOG_PUPDR
      *(volatile unsigned int *)(0x4002180Cu) |= (0b11u)<<(2*12);   //reserved

   //PE 5
      if(! CheckClockStatus(5)) ClockEnable(5);
      //GPIOE_MODER
      *(volatile unsigned int *)(0x40021000u) &= ~((0b11u)<<(2*5));
      *(volatile unsigned int *)(0x40021000u) |= (0b01u)<<(2*5); //output mode
      //GPIOE_OSPEEDR
      *(volatile unsigned int *)(0x40021008u) |= (0b11u)<<(2*5); //very high
      //GPIOE_PUPDR
      *(volatile unsigned int *)(0x4002100Cu) |= (0b11u)<<(2*5); //reserved

   //PE 4
      if(! CheckClockStatus(5)) ClockEnable(5);
      //GPIOE_MODER
      *(volatile unsigned int *)(0x40021000u) &= ~((0b11u)<<(2*4));
      *(volatile unsigned int *)(0x40021000u) |= (0b01u)<<(2*4); //output mode
      //GPIOE_OSPEEDR
      *(volatile unsigned int *)(0x40021008u) |= (0b11u)<<(2*4); //very high
      //GPIOE_PUPDR
      *(volatile unsigned int *)(0x4002100Cu) |= (0b11u)<<(2*4); //reserved

   //PG 10
      if(! CheckClockStatus(7)) ClockEnable(7);
      //GPIOG_MODER
      *(volatile unsigned int *)(0x40021800u) &= ~((0b11u)<<(2*10));
      *(volatile unsigned int *)(0x40021800u) |= (0b01u)<<(2*10); //output mode
      //GPIOG_OSPEEDR
      *(volatile unsigned int *)(0x40021808u) |= (0b11u)<<(2*10); //very high
      //GPIOG_PUPDR
      *(volatile unsigned int *)(0x4002180Cu) |= (0b11u)<<(2*10);   //reserved

      TurnOffAllLed();
}

void TurnOnLEDbyBit(unsigned char data)
{
	TurnOffAllLed();

   //PG 12
      //GPIOG_BSRR
      *(volatile unsigned int *)(0x40021818u) &= ~((0x01u)<<12);    //BS12 = 0
      *(volatile unsigned int *)(0x40021818u) |= (data & 0x01u)<<(12+16);//BR12 = 1

   //PE 5
      //GPIOE_BSRR
      *(volatile unsigned int *)(0x40021018u) &= ~((0x01u)<<5);    //BS5 = 0
      *(volatile unsigned int *)(0x40021018u) |= (data >> 1 & 0x01u)<<(5+16);   //BR5 = 1

   //PE 4
      //GPIOE_BSRR
      *(volatile unsigned int *)(0x40021018u) &= ~((0x01u)<<4);    //BS4 = 0
      *(volatile unsigned int *)(0x40021018u) |= (data >> 2 & 0x01u)<<(4+16);   //BR4 = 1

   //PG 10
      //GPIOG_BSRR
      *(volatile unsigned int *)(0x40021818u) &= ~((0x01u)<<10);    //BS10 = 0
      *(volatile unsigned int *)(0x40021818u) |= (data >> 3 & 0x01u)<<(10+16);//BR10 = 1

}

void TurnOffAllLed()
{
   //PG 12
      //GPIOG_BSRR
      *(volatile unsigned int *)(0x40021818u) &= ~((0x01u)<<(12+16));   //BR12 = 0
      *(volatile unsigned int *)(0x40021818u) |= (0x01u)<<12;             //BS12 = 1

   //PE 5
      //GPIOE_BSRR
      *(volatile unsigned int *)(0x40021018u) &= ~((0x01u)<<(5+16));   //BR5 = 0
      *(volatile unsigned int *)(0x40021018u) |= (0x01u)<<5;           //BS5 = 1

   //PE 4
      //GPIOE_BSRR
      *(volatile unsigned int *)(0x40021018u) &= ~((0x01u)<<(4+16));   //BR4 = 0
      *(volatile unsigned int *)(0x40021018u) |= (0x01u)<<4;           //BS4 = 1

   //PG 10
      //GPIOG_BSRR
      *(volatile unsigned int *)(0x40021818u) &= ~((0x01u)<<(10+16));   //BR10 = 0
      *(volatile unsigned int *)(0x40021818u) |= (0x01u)<<10;             //BS10 = 1

}

void GpioEnable(
		)
{
	//ard_d0 pc7
		if(! CheckClockStatus(3)) ClockEnable(3);
		//GPIOC_MODER
		*(volatile unsigned int *)(0x40020800u) &= ~((0b11u)<<(2*7));
		*(volatile unsigned int *)(0x40020800u) |= (0b01u)<<(2*7); //output mode
		//GPIOC_OSPEEDR
		*(volatile unsigned int *)(0x40020808u) |= (0b11u)<<(2*7); //very high
		//GPIOC_PUPDR
		*(volatile unsigned int *)(0x4002080Cu) &= ~((0b11u)<<(2*7));
		*(volatile unsigned int *)(0x4002080Cu) |= (0b01u)<<(2*7);   //pull-up

	//ard_d1 pc6
		if(! CheckClockStatus(3)) ClockEnable(3);
		//GPIOC_MODER
		*(volatile unsigned int *)(0x40020800u) &= ~((0b11u)<<(2*6));
		*(volatile unsigned int *)(0x40020800u) |= (0b01u)<<(2*6); //output mode
		//GPIOC_OSPEEDR
		*(volatile unsigned int *)(0x40020808u) |= (0b11u)<<(2*6); //very high
		//GPIOC_PUPDR
		*(volatile unsigned int *)(0x4002080Cu) &= ~((0b11u)<<(2*6));
		*(volatile unsigned int *)(0x4002080Cu) |= (0b01u)<<(2*6);   //pull-up

	//ard_d2 pg6
		if(! CheckClockStatus(7)) ClockEnable(7);
		//GPIOG_MODER
		*(volatile unsigned int *)(0x40021800u) &= ~((0b11u)<<(2*6));
		*(volatile unsigned int *)(0x40021800u) |= (0b01u)<<(2*6); //output mode
		//GPIOG_OSPEEDR
		*(volatile unsigned int *)(0x40021808u) |= (0b11u)<<(2*6); //very high
		//GPIOG_PUPDR
		*(volatile unsigned int *)(0x4002180Cu) &= ~((0b11u)<<(2*6));
		*(volatile unsigned int *)(0x4002180Cu) |= (0b01u)<<(2*6);   //pull-up

	for(int i = 1; i <=4; i++)
		ResetGpio(i);
}

void SetGpio(unsigned char No)
{
   if (No == 1){ //ard_d0 pc7
      //GPIOG_BSRR
      *(volatile unsigned int *)(0x40020818u) &= ~((0x01u)<<(7+16));   //BR7 = 0
      *(volatile unsigned int *)(0x40020818u) |= (0x01u)<<7;           //BS7 = 1
   }
   else if(No == 2){ //ard_d1 pc6
      //GPIOE_BSRR
      *(volatile unsigned int *)(0x40020818u) &= ~((0x01u)<<(6+16));   //BR6 = 0
      *(volatile unsigned int *)(0x40020818u) |= (0x01u)<<6;           //BS6 = 1
   }
   else if(No == 3){ //ard_d2 pg6
      //GPIOG_BSRR
      *(volatile unsigned int *)(0x40021818u) &= ~((0x01u)<<(6+16));   //BR6 = 0
      *(volatile unsigned int *)(0x40021818u) |= (0x01u)<<6;           //BS6 = 1
   }
}

void ResetGpio(unsigned char No)
{
   if (No == 1){ //ard_d0 pc7
      //GPIOG_BSRR
      *(volatile unsigned int *)(0x40020818u) &= ~((0x01u)<<7);    	//BS7 = 0
      *(volatile unsigned int *)(0x40020818u) |= (0x01u)<<(7+16);	//BR7 = 1
   }
   else if(No == 2){ //ard_d1 pc6
      //GPIOE_BSRR
      *(volatile unsigned int *)(0x40020818u) &= ~((0x01u)<<6);    	//BS6 = 0
      *(volatile unsigned int *)(0x40020818u) |= (0x01u)<<(6+16);   //BR6 = 1
   }
   else if(No == 3){ //ard_d2 pg6
	   //GPIOG_BSRR
	   *(volatile unsigned int *)(0x40021818u) &= ~((0x01u)<<6);   	//BS6 = 0
	   *(volatile unsigned int *)(0x40021818u) |= (0x01u)<<(6+16);  //BR6 = 1
   }
}

void ButtonEnable()
{
	//PG 3 / sw3 -> but_user2 pg3 / in-board
		if(! CheckClockStatus(7)) ClockEnable(7);
		*(volatile unsigned int*)(0x40021800U) &= ~((0b11u)<<(2*3)); //input
		*(volatile unsigned int*)(0x40021808U) &= ~((0b11u)<<(2*3)); //low speed
		*(volatile unsigned int*)(0x4002180CU) &= ~((0b11u)<<(2*3)); //no pull-up,down

	//ard_d3 pb4 / out-board
		if(! CheckClockStatus(2)) ClockEnable(2);
		//GPIOB_MODER
		*(volatile unsigned int *)(0x40020400u) &= ~((0b11u)<<(2*4)); //input mode
		//GPIOB_OSPEEDR
		*(volatile unsigned int *)(0x40020408u) &= ~((0b11u)<<(2*4)); //low speed
		//GPIOB_PUPDR
		*(volatile unsigned int *)(0x4002040Cu) &= ~((0b11u)<<(2*4)); //no pull-up,pull-down
}

int isButtonPressed(unsigned int No)
{
	if (No == 1) {	//PG 3 / sw3 -> but_user2 pg3
		if(*(volatile unsigned int *)(0x40021810u) & (0x01<<3))
			return 1;
		else
			return 0;
	}
	else if (No == 2) {	//ard_d3 pb4
		if(*(volatile unsigned int *)(GPIO_B + _IDR) & (0x01<<4))
			return 1;
		else
			return 0;
	}

	return 0;
}

//클락을 이용하여 하나의 비트 값을 할당
void WriteOneClk(unsigned int v)
{
	if(v)	SetGpio(3);		//data

	SetGpio(1);				//clk
	ResetGpio(1);

	ResetGpio(3);
}

//전체 8비트 값을 할당
void WriteData(unsigned int data)
{
	WriteOneClk(data&0x80u);
	WriteOneClk(data&0x40u);
	WriteOneClk(data&0x20u);
	WriteOneClk(data&0x10u);
	WriteOneClk(data&0x08u);
	WriteOneClk(data&0x04u);
	WriteOneClk(data&0x02u);
	WriteOneClk(data&0x01u);

	ResetGpio(2);
	SetGpio(2);
}

unsigned int DsHigh()
{
	WriteData(0b1000000);
}

unsigned int DsLow()
{
	WriteData(0);
}

unsigned int ClkHigh()
{
	WriteData(0b0010000);
}

unsigned int ClkLow()
{
	WriteData(0b1101111);
}

unsigned int RestorageHigh()
{
	WriteData(0b0100000);
}

unsigned int RestorageLow()
{
	WriteData(0b1011111);
}

unsigned int WriteToSSOne(unsigned int v)
{
	//첫번째 시프트레지스터에 연결된 두번째 시프트레지스터를 제어하기위한 함수들
	if(v)	DsHigh();

	ClkHigh();
	ClkLow();

	DsLow();
}

//Write To Seven-Segment
unsigned int WriteToSS (unsigned int ss)
{
	WriteToSSOne(ss&0x80u);
	WriteToSSOne(ss&0x40u);
	WriteToSSOne(ss&0x20u);
	WriteToSSOne(ss&0x10u);
	WriteToSSOne(ss&0x08u);
	WriteToSSOne(ss&0x04u);
	WriteToSSOne(ss&0x02u);
	WriteToSSOne(ss&0x01u);

	RestorageHigh();
	RestorageLow();
}

void SevenSeg(int No)
{
	if(No == 0)
		WriteToSS(0b10000000u);
	else if (No == 1)
		WriteToSS(0b11110010u);
	else if (No == 2)
		WriteToSS(0b01001000u);
	else if (No == 3)
		WriteToSS(0b01100000u);
	else if (No == 4)
		WriteToSS(0b00110010u);
	else if (No == 5)
		WriteToSS(0b00100100u);
	else if (No == 6)
		WriteToSS(0b00000100u);
}

//전역변수 btn_flag를 이용하여 버튼 동작 구현 / 누르고 뗀 순간에 함수 종료 및 새로운 상태 전달
void DelayWithBTN(unsigned int duration)
{
	volatile unsigned int delay;
	for(delay=0; delay<=duration*430000; delay++) // 430000 == 약 0.1s
	{
		if(btn_flag1 == BTN_NOT_PRESSED && isButtonPressed(1)) {
			btn_flag1 = BTN_PRESSED;
		}
		else if(btn_flag1 == BTN_PRESSED && !isButtonPressed(1)) {
			btn_flag1 = BTN_NOT_PRESSED;
			switching_flag = RE;
			return;
		}

		if(btn_flag2 == BTN_NOT_PRESSED && !isButtonPressed(2)) {
			btn_flag2 = BTN_PRESSED;
		}
		else if(btn_flag2 == BTN_PRESSED && isButtonPressed(2)) {
			btn_flag2 = BTN_NOT_PRESSED;
			switching_flag = NEXT;
			return;
		}
	}
}

void MyApp()
{
	//순서대로 바이너리카운터, 상태별 남은 시간 초 단위 카운터, 깜박임 효과를 위한 변수
	unsigned int bi_cnt = 0, sec_cnt = 7, blink_bit = 1;

	LedEnable();
	GpioEnable();
	ButtonEnable();

	//전역변수 초기화 / 버튼 1(내부), 버튼 2(외부) 플래그, 스위칭플래그, 현재 진행상태를 나타내는 변수
	btn_flag1 = btn_flag2 = switching_flag = state = 0;

	while(1)
	{
		if(switching_flag) {
			if(switching_flag == NEXT) {		//다음 상태로 변화
				state = (state+1) % 8;

				if( state == 0) {
					bi_cnt = (bi_cnt+1) % 16;
				}
				else if (state == 1) {
					SevenSeg(0);
				}
			}
			else if (switching_flag == RE) {	//리셋 시
				bi_cnt = 0;
				state = 0;
			}

			//시프트레지스터 값 변경
			//state 값 하위 3비트와 4번째 비트를 이용 / 4번째 비트에 1을 주어야 횡단보도 신호등이 켜짐
			WriteData(state | 0b1000);

			if(state % 2 == 0)	sec_cnt = 7;
			else				sec_cnt = 2;

			//내부 LED 제어
			TurnOnLEDbyBit(bi_cnt);
			switching_flag = 0;		//스위칭변수 초기화
		}

		sec_cnt--;
		if(state == 0) {			//수직차선 초록불
			//7 segment 값 변경
			SevenSeg(sec_cnt);
			WriteData(state | 0b1000);
		}

		if (state == 0 || state == 4) {		//횡단보도 신호등이 초록불일 때
			//깜빡임구현
			if(sec_cnt < 2) {
				for(int i = 0; i < 10; i++) {
					blink_bit = (blink_bit+1) % 2;		//0, 1 반복
					WriteData(state | blink_bit << 3);	//위에서 언급했던 4번째 비트
					DelayWithBTN(1); // 0.1s
				}
			}
			else
				DelayWithBTN(10);	//1s
		}
		else {
			DelayWithBTN(10);
		}

		if(sec_cnt == 0) {			//시간 경과시 다음 상태로
			switching_flag = NEXT;
		}
	}
}
