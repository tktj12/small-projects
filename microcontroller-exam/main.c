#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "mc_ws2812.h"

#define F_CPU 16000000UL
#define FOSC  16000000UL
#define BAUD_9600 103
#define STX 0x02
#define ETX 0x03

#define OFF 0
#define ON 1
#define LED 2
#define NEO 3
#define SEL 4

int ADC_mode = OFF, sint_flag = 0, decode = 0;
char HW[9];
unsigned char len, len_cnt, command, param;

void cc(unsigned char aa)
{
	while(!(UCSR1A << UDRE1));
	UDR1 = aa;
}

ISR(USART1_RX_vect)
{
	unsigned char uart_rcv = UDR1;
	if(decode == 0 && uart_rcv == STX) {
		decode = 1;
	}
	else if (decode == 1) {
		len_cnt = len = uart_rcv - 2;
		decode = 2;
	}
	else if (decode == 2) {
		HW[len_cnt - len] = uart_rcv;
		len--;
		if(len == 0){
			decode = 3;
		}
	}
	else if (decode == 3) {
		command = uart_rcv;
		decode = 4;
	}
	else if (decode == 4) {
		param = uart_rcv;
		decode = 5;
	}
	else if (decode == 5) {
		if(uart_rcv == ETX) {
			sint_flag = 1;
		}
		else {
			printf("RX error\n");
		}
		decode = 0;
	}
	cc(uart_rcv);
	sei();
}

unsigned int getAdcData(unsigned char aIn)
{
	volatile unsigned int result;
	ADMUX = aIn;
	ADMUX |= 1<<REFS0;
	ADCSRA = 0x01<<ADEN | 0x07; //ADC Enable, ADC prescaler : 128
	ADCSRA |= 1<<ADSC;
	while(!(ADCSRA & (1<<ADIF)) );
	result=ADCL + (ADCH<<8);
	ADCSRA = 0x00;
	
	return result;
}

int main(){
	cli();
	DDRB = 0x40;
	DDRC = 0xFF;
	DDRF &= ~0x01;

	UBRR1H = (unsigned char) (BAUD_9600 >>8) ;
	UBRR1L = (unsigned char) BAUD_9600;
	UCSR1B = (1<< TXEN1)|  (1<< RXEN1);
	UCSR1B |= (1<< RXCIE1); //RX Complete Interrupt Enable
	UCSR1C = (1<< UCSZ11) | (1<< UCSZ10) | (0x01<<UPM10) | (0x01<<UPM11);
	sei();
	
	rgbw_color color_arr[16] = {0,} , neo_off[16] = {0,};
	
	unsigned int data;
	int i;
	
	while(1)
	{
		if(sint_flag) {
			if(strstr(HW, "LED") != NULL){
				ws2812_setleds_rgbw_PB(6, neo_off, 16);
				ADC_mode = OFF;
				if(command == 0x01) {//led on off
					led_on_off(param);
				}
				else if(command == 0x02) {//led alt
					led_alt(param);
				}
				else if(command == 0x03) {//led shift
					led_shift(param);
				}
			}
			else if (strstr(HW, "ADC") != NULL){
				if(command == 0x01) {
					if(ADC_mode == OFF || ADC_mode == SEL)
						ADC_mode = ON;
				}
				else if(command == 0x02) {
					ADC_mode = OFF;
				}
				else if(command == 0x03) {
					if(ADC_mode == ON || ADC_mode == NEO) {
						ADC_mode = LED;
					}
				}
				else if(command == 0x04) {
					if(ADC_mode == ON || ADC_mode == LED) {
						ADC_mode = NEO;
					}
				}
			}
			else if (strstr(HW, "NEOPIXEL") != NULL){
				ADC_mode = OFF;
				if(command == 0x10) {//all on of
					neo_on_off(param);
				}
				else if(command == 0x20) {//shift
					neo_shift(param);
				}
				else if(command == 0x40) {//bright
					neo_breath(param);
				}
				else if(command == 0x80) {//selective on
					memset(color_arr,0,16*4);
					color_arr[param-1].blue = 20;
					if(param > 8) {
						color_arr[param-9].blue = 20;
					}
					else {
						color_arr[param+7].blue = 20;
					}
					ADC_mode = SEL;
				}
			}
			memset(HW, NULL, 9);
			command = sint_flag = len = len_cnt = param = 0;
		}
		if(ADC_mode == NEO) {
			data = getAdcData(0) / 64;
			for(i = 0; i <= data; i++)
				color_arr[i].blue = 20;
			for(;i < 16; i++)
				color_arr[i].blue = 0;

			PORTC = 0x00;
			ws2812_setleds_rgbw_PB(6,color_arr,16);
		}
		else if(ADC_mode == LED) {
			data = getAdcData(0) / 128;
			for(i = 0; i <= data; i++)
				PORTC |= 1<<i;
			for(;i < 8; i++)
				PORTC &= ~(1<<i);
			
			ws2812_setleds_rgbw_PB(6, neo_off, 16);
		}
		else if(ADC_mode == SEL) {
			PORTC = 0x00;
			ws2812_setleds_rgbw_PB(6,color_arr,16);
		}
		else{
			ws2812_setleds_rgbw_PB(6, neo_off, 16);
			PORTC = 0x00;
		}
	}
	
}

void led_on_off(int n)
{
	for(int i = 0; i < n; i++)
	{
		PORTC = 0xFF;
		_delay_ms(300);
		PORTC = 0x00;
		_delay_ms(300);
	}
}

void led_alt(int n)
{
	for(int i = 0; i < n; i++)
	{
		PORTC = 0x0F;
		_delay_ms(300);
		PORTC = ~0x0F;
		_delay_ms(300);
	}
}

void led_shift(int n)
{
	int flag;
	for(int i = 0; i < n; i++)
	{
		flag = 0;
		for(int j = 0; j >= 0; flag ? j-- : j++)
		{
			PORTC = 0x01<<j;
			_delay_ms(100);
			if(j == 7)
				flag = 1;
		}
	}
	PORTC = 0x00;
}

void neo_on_off(int n)
{
	rgbw_color on[16] = {0,} , off[16] = {0,};
	for(int i = 0; i < 16; i++){
		on[i].blue = 20;
	}
	for(int i = 0; i < n; i++)
	{
		ws2812_setleds_rgbw_PB(6, on, 16);
		_delay_ms(300);
		ws2812_setleds_rgbw_PB(6, off, 16);
		_delay_ms(300);
	}
}

void neo_shift(int n)
{
	rgbw_color arr[16] = {0,};
	int flag;
	
	for(int i = 0; i < n; i++)
	{
		flag = 0;
		for(int j = 0; j >= 0; flag ? j-- : j++)
		{
			memset(arr,0,4*16);
			arr[j].blue = 20;
			ws2812_setleds_rgbw_PB(6, arr, 16);
			if(j == 15) {
				flag = 1;
			}
			_delay_ms(50);
		}
	}
}

void neo_breath(int n)
{
	rgbw_color arr[16] = {0,};
	int flag;
	
	for(int i = 0; i < n; i++)
	{
		flag = 0;
		for(int j = 0; j >= 0; flag ? j-- : j++)
		{
			for(int k = 0; k < 16; k++)
			{
				arr[k].blue = j;
			}
			if(j == 100){
				flag = 1;
			}
			ws2812_setleds_rgbw_PB(6, arr, 16);
			_delay_ms(5);
		}
	}
}