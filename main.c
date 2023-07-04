/*
 * main.c
 *
 * Created: 7/4/2023 8:59:47 AM
 *  Author: Mwaniki
 */ 

#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include "LCD_4bit.h"
#include <avr/interrupt.h>

unsigned char keypad[4][4] = {{'7','8','9','/'},
							  {'4','5','6','*'},
							  {'1','2','3','-'},
							  {'C','0','=','+'}};

unsigned char check_keypad();
void keypad_init();


void keypad_init(){
	//DDRB = 0xFF;
	DDRD |= (0<<PD2); //PORTD as input
	PORTD |= (1<<PD2);//Pull up resistor
	
	
	GICR = 1<<INT0; //Enable INT0
	MCUCR = 1<<ISC01 |1<<ISC00;//Trigger int on rising edge
	sei();	//enable Global Interrupt
}

ISR(INT0_vect){
	//to read values from keypresses
	
	unsigned char key_val= check_keypad();
	LCD_Char(key_val);
}

int main(void)
{	
	sei();
	LCD_Init();
	keypad_init();
	
    while(1)
    {
		
    }
}

unsigned char check_keypad(){
	
	char input_val = PINB; 
	int row = input_val/4;
	int col = input_val%4;
	
	if((input_val>=0) & (input_val<16)) return (keypad[row][col]);
	else return 0;
}