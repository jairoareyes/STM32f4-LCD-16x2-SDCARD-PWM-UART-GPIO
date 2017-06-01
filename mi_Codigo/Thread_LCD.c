#include "cmsis_os.h"  
#include "stm32f4xx.h"
#include "Thread_lcd.h"
#include "Thread_Uart.h"
#include "Thread_SD.h"


extern ADC_HandleTypeDef hadc1;
extern osThreadId tid_Thread_uart; 
extern osThreadId tid_Thread_motores;  
extern osThreadId tid_Thread_rf; 
char Dato_LCD1[16];
char Dato_LCD2[16];
int fila2,columna2; 
char dato2[16];
	
void ImprimirLCD(int fila, int columna, char dato[]);
void Thread_LCD (void const *argument);                             // thread function
osThreadId tid_Thread_LCD;                                          // thread id
osThreadDef (Thread_LCD, osPriorityNormal, 1,0);                   // thread object

osMutexDef (LCD_mutex);    // Declare mutex
osMutexId  (LCD_mutex_id); // Mutex ID

int Init_Thread_LCD (void) {
  tid_Thread_LCD = osThreadCreate (osThread(Thread_LCD), NULL);
  if (!tid_Thread_LCD) return(-1);
	LCD_mutex_id = osMutexCreate(osMutex(LCD_mutex));	
  return(0);
}
/*    RS->PE11
			EN->PE13
			D4->PD13
			D5->PD11
			D6->PB15
			D7->PB13
*/

void Thread_LCD(void const *argument) {
	uint32_t adc_data;
	double voltaje;

	static uint32_t Tiempo_inicio_lcd,Tiempo_dif_lcd;
	static uint32_t Tiempo_final_lcd;
	char nomTXT_lcd[]="DatosLCD.txt";
	char Dato_SD_lcd[40];
	
	LCD_begin4BIT(GPIOD, GPIOB,13,11,15,13);
	LCD_clear();
	ImprimirLCD(1,1,"      EAGLE    ");
	ImprimirLCD(2,1,"   DaemonBots  ");
  HAL_ADC_Start_DMA(&hadc1, &adc_data,1);  //de 0 a 1023
	osDelay(3000);
	
	while (1) {		
		Tiempo_inicio_lcd=HAL_GetTick();
		voltaje=((adc_data*10.75)/1023);
    if(voltaje<=6.8){
		LCD_setCursor(2,1);	
		LCD_print("  Bateria Baja! ");
		osThreadTerminate (tid_Thread_uart);
		osThreadTerminate (tid_Thread_motores);
		osThreadTerminate (tid_Thread_rf);		
		osThreadTerminate (tid_Thread_LCD);
		}		
		sprintf (Dato_LCD1, "Voltaje: %f", voltaje);
		ImprimirLCD(2,1,Dato_LCD1);
		osDelay(500);
		ImprimirLCD(1,1,Dato_LCD2);
		osDelay(500);		
		Tiempo_final_lcd=HAL_GetTick();  
		Tiempo_dif_lcd = Tiempo_final_lcd - Tiempo_inicio_lcd;
		sprintf (Dato_SD_lcd, "Start,%d,End,%d,Dif,%d",Tiempo_inicio_lcd,Tiempo_final_lcd, Tiempo_dif_lcd);		
		ImprimirSD(nomTXT_lcd,Dato_SD_lcd);
	}
}

void ImprimirLCD(int fila, int columna, char dato[]){
	osMutexWait(LCD_mutex_id, osWaitForever);	
	LCD_setCursor(fila,columna);
	LCD_print(dato);	
	osMutexRelease(LCD_mutex_id); 
}
void EstadoMot(uint8_t dato_mot){
	switch (dato_mot){
		case 1:
		sprintf (Dato_LCD2, " Adelante e Izq ");		
		break;
		case 2:
		sprintf (Dato_LCD2, " Adelante y Der ");	
		break;
		case 3:
		sprintf (Dato_LCD2, "     Adelante   ");	
		break;
		case 4:
		sprintf (Dato_LCD2, "    Izquierda   ");	
		break;
		case 5:
		sprintf (Dato_LCD2, "     Derecha    ");	
		break;
		case 6:
		sprintf (Dato_LCD2, "      Atras    ");	
		break;
		case 7:
		sprintf (Dato_LCD2, "    Detenerse   ");	
		break;
		default:
		break;	
	}
}