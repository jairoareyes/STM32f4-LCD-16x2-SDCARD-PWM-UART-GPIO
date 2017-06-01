#include "stm32f4xx_hal.h"
int Init_Thread_LCD (void);
void Thread_LCD (void const *argument); 
void ImprimirLCD(int fila, int columna, char dato[]);
void EstadoMot(uint8_t dato_mot);


void LCD_begin4BIT(GPIO_TypeDef* PORT_MSBs4y5, GPIO_TypeDef* PORT_MSBs6y7,uint16_t D4, uint16_t D5, uint16_t D6, uint16_t D7);
// Function(3): Print to Display
void LCD_print(char string[]);
// Function(4): Clear display
void LCD_clear(void);
// Function(5): Set Cursor position
void LCD_setCursor(uint8_t row, uint8_t col);
// Function(6): Enable two lines
void LCD_TwoLines(void);

void LCD_OneLine(void);
// Function(7): Cursor ON/OFF
void LCD_noCursor(void);
void LCD_cursor(void);
// Function(8): Blinking cursor
void LCD_noBlink(void);

void LCD_blink(void);
// Function(9): Display ON/OFF
void LCD_noDisplay(void);

void LCD_display(void);
// Function(10): Shift Display, right or left
void LCD_shiftToRight(uint8_t num);
void LCD_shiftToLeft(uint8_t num);

	
