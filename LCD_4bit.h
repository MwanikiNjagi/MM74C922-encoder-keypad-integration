#ifndef __LCD_4BIT__
#define __LCD_4BIT__

#include <avr/io.h>
#include <util/delay.h>
#include "typedef.h"

#define RS PD0
#define EN PD1

#define LCD_Dir DDRC
#define LCD_Port PORTC

void LCD_Cmd(uint8 cmd)
{
	LCD_Port = (LCD_Port & 0x0F) | (cmd & 0xF0);/* Sending upper nibble */
	LCD_Port &= ~ (1<<RS); /* RS=0, command reg. */
	LCD_Port |= (1<<EN); /* Enable pulse ON */
	_delay_us(1);
	LCD_Port &= ~ (1<<EN); /* Enable pulse OFF */
	_delay_us(200);
	/*Sending the second nibble of data (Lower 4 bits)*/
	LCD_Port = (LCD_Port & 0x0F) | (cmd << 4);/* Sending lower nibble */
	LCD_Port |= (1<<EN); /* Enable pulse ON */
	_delay_us(1);
	LCD_Port &= ~ (1<<EN); /* Enable pulse OFF */
	_delay_ms(2);
}
void LCD_Char(uint8 char_data)
{
	/*Sending the first nibble of data (Higher 4 bits)*/
	LCD_Port = (LCD_Port & 0x0F) | (char_data & 0xF0);/* Sending upper nibble */
	LCD_Port |= (1<<RS); /* RS=1, data reg. */
	LCD_Port |= (1<<EN); /* Enable pulse ON */
	_delay_us(1);
	LCD_Port &= ~ (1<<EN); /* Enable pulse OFF */
	_delay_us(200);
	/*Sending the second nibble of data (Lower 4 bits)*/
	LCD_Port = (LCD_Port & 0x0F) | (char_data << 4); /* Sending lower nibble */
	LCD_Port |= (1<<EN); /* Enable pulse ON */
	_delay_us(1);
	LCD_Port &= ~ (1<<EN); /* Enable pulse OFF */
	_delay_ms(2);
}
void LCD_Str(const char *str)
{	
	int i;
	for(i=0;str[i]!=0;i++)
	{
		LCD_Char(str[i]);
	}
	
}
void LCD_Init(void)
{
	LCD_Dir = 0xFF; /* Make LCD command port direction as output pins*/
	_delay_ms(20); /* LCD Power ON delay always > 15ms */
	LCD_Cmd(0x02); /* Return display to its home position */
	LCD_Cmd(0x28); /* 2 line 4bit mode */
	LCD_Cmd(0x0C); /* Display ON Cursor OFF */
	LCD_Cmd(0x06); /* Auto Increment cursor */
	LCD_Cmd(0x01); /* Clear display */
}

void LCD_Clear(void)
{
	LCD_Cmd(0x01); /* clear display */
	LCD_Cmd(0x02); /* Return display to its home position*/
}

void LCD_move_cursor(uint8 row, uint8 column)
{
	//uint8 r = 0;
	uint8 c = 0;
	
	if( (row>0) && (row<5) && (column>0) && (column<21) )
	{
		if (1 == row)
		{
			c = (0x80 + column - 1);
		}
		else if (2 == row)
		{
			c = (0xC0 + column - 1);
		}
		else if (3 == row)
		{
			c = (0x90 + column - 1);
		}
		else
		{
			c = (0xD0 + column - 1);
		}
	}
	else
	{
		c = 0x80;
	}
	LCD_Cmd(c);
}

#endif //__LCD_4BIT__