#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "stm32f4xx.h"
#include  <stdbool.h>
#include "stm32f4xx_hal.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Thread_lcd.h"


#define ENTRY_MODE							0x04
#define DISPLAY_CONTROL					0x08
#define DISPLAYandCURSOR_SHIFT	0x10


static void writeCommand(uint8_t command);
// Function(2): RS ans E pins
static void RS_pin(bool state);
static void E_pin(bool state);
// Function(3): Switch mode of operation
static void switchMode(bool Mode);
// Function(4): Write a single Character to the display
static void LCD_writeChar(char text);
static void setDataLength(bool length);



static GPIO_TypeDef* PORT_MSB1;
static GPIO_TypeDef* PORT_MSB2;										// MSBs D5, D6, D7 and D8 PORT
static uint8_t D4_PIN, D5_PIN, D6_PIN, D7_PIN;	// MSBs D5, D6, D7 and D8 pins
static uint8_t DisplayControl = 0x0F;
static uint8_t FunctionSet = 0x38;



static void RS_pin(bool state)
{
	if(state)
	{
		HAL_GPIO_WritePin(GPIOE,  GPIO_PIN_11, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOE,  GPIO_PIN_11, GPIO_PIN_RESET);
	}
}
static void E_pin(bool state)
{
	if(state)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, GPIO_PIN_RESET);
	}
}
// Function(1): Write command
static void writeCommand(uint8_t command)
{
	uint8_t LSB_data, MSB_data;
	LSB_data = 0x0F&command;
	MSB_data = 0x0F&(command>>4);

	// Reset pins from D4 to D7
		PORT_MSB1->ODR &= ~((1UL<<D4_PIN)|(1UL<<D5_PIN));
		PORT_MSB2->ODR &= ~((1UL<<D6_PIN)|(1UL<<D7_PIN));
		//PORT_LSB->ODR &= ~((1UL<<D0_PIN)|(1UL<<D1_PIN)|(1UL<<D2_PIN)|(1UL<<D3_PIN));
		// Write new command to D4 to D7
		PORT_MSB1->ODR |= (((MSB_data>>0)&1UL)<<D4_PIN);
		PORT_MSB1->ODR |= (((MSB_data>>1)&1UL)<<D5_PIN);
		PORT_MSB2->ODR |= (((MSB_data>>2)&1UL)<<D6_PIN);
		PORT_MSB2->ODR |= (((MSB_data>>3)&1UL)<<D7_PIN);
		for(uint32_t i=0; i<100; i++);
		E_pin(true);
		for(uint32_t i=0; i<900; i++);
		E_pin(false);
		//for(uint32_t i=0; i<70; i++);
		// Reset pins from D4 to D7
		PORT_MSB1->ODR &= ~((1UL<<D4_PIN)|(1UL<<D5_PIN));
		PORT_MSB2->ODR &= ~((1UL<<D6_PIN)|(1UL<<D7_PIN));
		// Write new command to D4 to D7
		PORT_MSB1->ODR |= (((LSB_data>>0)&1UL)<<D4_PIN);
		PORT_MSB1->ODR |= (((LSB_data>>1)&1UL)<<D5_PIN);
		PORT_MSB2->ODR |= (((LSB_data>>2)&1UL)<<D6_PIN);
		PORT_MSB2->ODR |= (((LSB_data>>3)&1UL)<<D7_PIN);
		for(uint32_t i=0; i<100; i++);
		E_pin(true);
		for(uint32_t i=0; i<900; i++);
		E_pin(false);
}

// Funciton(3): Change mode
static void switchMode(bool Mode)
{
	RS_pin(false);
	osDelay(50);
	writeCommand(0x30);
	osDelay(5);
	writeCommand(0x30);
  osDelay(5);
	writeCommand(0x30);
	osDelay(5);
	if(Mode)
	{
		writeCommand(0x30);
	}
	else
	{
		writeCommand(0x20);
	}
	osDelay(5);
}
// Function(4): Write a single Character to the display
static void LCD_writeChar(char text)
{
	RS_pin(true);
	writeCommand(text);
}
static void setDataLength(bool length)
{
	RS_pin(false);
	if(length)
	{
		FunctionSet |= (0x10);
		writeCommand(FunctionSet);
	}
	else
	{
		FunctionSet &= ~(0x10);
		writeCommand(FunctionSet);
	}
}

void LCD_begin4BIT(GPIO_TypeDef* PORT_MSBs4y5, GPIO_TypeDef* PORT_MSBs6y7,uint16_t D4, uint16_t D5, uint16_t D6, uint16_t D7)
{
	PORT_MSB1 = PORT_MSBs4y5;
	PORT_MSB2	= PORT_MSBs6y7;
	D4_PIN = D4;
	D5_PIN = D5;
	D6_PIN = D6;
	D7_PIN = D7;
	// Switch to 4bits mode
	osDelay(50);
	switchMode(false);
	setDataLength(false);
	writeCommand(0x0F);
	osDelay(5);
	writeCommand(0x0F);
	osDelay(5);
	LCD_clear();
	LCD_TwoLines();
}
void LCD_print(char string[])
{
	
	for(uint8_t i=0;  i< 16 && string[i]!=NULL; i++)
	{ 
		osDelay(10);
		LCD_writeChar(string[i]);
	}
}
// Function(4): Clear display
void LCD_clear(void)
{
	RS_pin(false);
	writeCommand(0x01);
	osDelay(2);
}
// Function (5): Set Cursor position
void LCD_setCursor(uint8_t row, uint8_t col)
{ osDelay(20);
	uint8_t maskData;
	RS_pin(false);
	maskData = (col-1)&0x0F;
	if(row==1)
	{
		maskData |= (0x80);
		writeCommand(maskData);
	}
	else
	{
		maskData |= (0xc0);
		writeCommand(maskData);
	}
}
// Function(6): Enable two lines
void LCD_TwoLines(void)
{
	RS_pin(false);
	FunctionSet |= (0x08);
	writeCommand(FunctionSet);
}
void LCD_OneLine(void)
{
	RS_pin(false);
	FunctionSet &= ~(0x08);
	writeCommand(FunctionSet);
}
// Function(7): Blinking cursor
void LCD_noBlink(void)
{
	RS_pin(false);
	DisplayControl &= ~(0x01);
	writeCommand(DisplayControl);
}
void LCD_blink(void)
{
	RS_pin(false);
	DisplayControl |= 0x01;
	writeCommand(DisplayControl);
}
// Function(8): Display ON/OFF
void LCD_noDisplay(void)
{
	RS_pin(false);
	DisplayControl &= ~(0x04);
	writeCommand(DisplayControl);
}
void LCD_display(void)
{
	RS_pin(false);
	DisplayControl |= (0x04);
	writeCommand(DisplayControl);
}
// Function(9): Cursor ON/OFF
void LCD_noCursor(void)
{
	RS_pin(false);
	DisplayControl &= ~(0x03);
	writeCommand(DisplayControl);
}
void LCD_cursor(void)
{
	RS_pin(false);
	DisplayControl |= (0x03);
	writeCommand(DisplayControl);
}
// Function(10): Shift Display or Cursor, right or left
void LCD_shiftToRight(uint8_t num)
{
	RS_pin(false);
	for(uint8_t i=0; i<num;i++)
	{
		writeCommand(0x1c);
	}
}
void LCD_shiftToLeft(uint8_t num)
{
	RS_pin(false);
	for(uint8_t i=0; i<num;i++)
	{
		writeCommand(0x18);
	}
}
