#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include <stdio.h>
#include "stm32f4xx.h"
#include "Thread_Uart.h"
#include "Thread_rf.h"
#include "Thread_motores.h"
#include "Thread_lcd.h"
#include "Thread_SD.h"
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Uart'
 *---------------------------------------------------------------------------*/
 extern UART_HandleTypeDef huart2;
 int ctrlRF;
 uint8_t uart_rx;
 uint8_t dato;
 int uart_recibido;
 
void ThreadUart (void const *argument);                             // thread function
osThreadId tid_Thread_uart;                                          // thread id
osThreadDef (ThreadUart, osPriorityNormal, 1, 0);                   // thread object

int Init_Thread_Uart (void) {

  tid_Thread_uart = osThreadCreate (osThread(ThreadUart), NULL);
  if (!tid_Thread_uart) return(-1);
  
  return(0);
}

void ThreadUart (void const *argument) {
	static uint32_t Tiempo_inicio_uart,Tiempo_dif_uart, Tiempo_final_uart;
	char nomTXT_uart[]="DatoUar.txt";
	char Dato_SD_uart[40];
	uint8_t Estado_uart=0;
	int PrimVez=0;
	ctrlRF=1;
	uart_recibido=0;	
	HAL_UART_Receive_IT(&huart2, &uart_rx, 1);
 while (1) {
 Tiempo_inicio_uart=HAL_GetTick();
		if(uart_recibido==1){
			uart_recibido=0;
			HAL_UART_Receive_IT(&huart2, &uart_rx, 1);// Lee el dato del UART 2
			dato=uart_rx;
			if(dato=='X'){
			ctrlRF = 0;
			}
			else if(dato=='x'){
			ctrlRF = 1;
			}
			if(ctrlRF==0){

				if (dato=='B'){//Reversa
					DatoMotores (0,200,200,0);
					PrimVez=0;
					Estado_uart=6;
			}
				else if (dato=='R'){//Derecha
					DatoMotores (0,100,0,200);
					PrimVez=0;
					Estado_uart=5;
			}
				else if (dato=='L'){//Izquierda
				DatoMotores (200,0,100,0);
					PrimVez=0;
					Estado_uart=4;
			}
				else if (dato=='F'){//Adelante
				DatoMotores (200,0,0,200);
				PrimVez=0;
				Estado_uart=3;
			}
				else if (dato=='I'){//Adelante y Derecha
				DatoMotores (50,0,0,250);
				PrimVez=0;
				Estado_uart=2;	
			}
				else if (dato=='G'){//Adelante y Izquierda
				DatoMotores (250,0,0,50);
				PrimVez=0;
				Estado_uart=1;
			}
				else if (dato=='W'||dato=='w'){//Giro
				DatoMotores (400,0,400,0);
				osDelay(200);	
				DatoMotores (1000,1000,1000,1000);
				osDelay(30);	
				DatoMotores (0,0,0,0);	
				PrimVez=0;
			}
				else if (dato=='U'||dato=='u'){//TURBO
				DatoMotores (500,0,0,500);
				osDelay(300);			
				PrimVez=0;
			}
				else if(dato=='V'||dato=='v'){//Patada
				DatoMotores (0,300,0, 300);
				osDelay(500);
				PrimVez=0;
			}
				else {
					if (PrimVez==0){
					PrimVez=1;
					DatoMotores (1000,1000,1000,1000);
					osDelay(20);
					DatoMotores (0,0,0,0);
					Estado_uart=7;						
					}
					else{
					DatoMotores (0,0,0,0);	
					}
				}
				EstadoMot(Estado_uart);
			}
		}
		Tiempo_final_uart=HAL_GetTick();  
		Tiempo_dif_uart = Tiempo_final_uart - Tiempo_inicio_uart;
		sprintf (Dato_SD_uart, "Start,%d,End,%d,Dif,%d",Tiempo_inicio_uart,Tiempo_final_uart, Tiempo_dif_uart);		
		ImprimirSD(nomTXT_uart,Dato_SD_uart);
		
  }

}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){

	if(huart->Instance==USART2)
	{
		uart_recibido=1;
	}

}

