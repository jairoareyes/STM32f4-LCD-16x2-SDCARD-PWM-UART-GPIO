#include "cmsis_os.h"   
#include "Thread_Uart.h"
#include "Thread_rf.h"
#include "Thread_motores.h"
#include "Thread_lcd.h"
#include "Thread_SD.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Motores'
 *---------------------------------------------------------------------------*/
 
int motA1=0,motA2=0,motB1=0, motB2=0;
 
void DatoMotores(int der1,int der2,int izq1, int izq2);
void ThreadMotores (void const *argument);                             // thread function
osThreadId tid_Thread_motores;                                          // thread id
osThreadDef (ThreadMotores, osPriorityNormal, 1, 0);                   // thread object

int Init_Thread_Motores (void) {

  tid_Thread_motores = osThreadCreate (osThread(ThreadMotores), NULL);
  if (!tid_Thread_motores) return(-1);
  
  return(0);
}

void ThreadMotores (void const *argument) {
	
	extern TIM_HandleTypeDef htim4;
	char nomTXT_mot[]="DatosMot.txt";
	static uint32_t Tiempo_inicio_mot,Tiempo_dif_mot,Tiempo_final_mot;
	char Dato_SD_mot[40];
	
  HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_1);      //Se inicializa el Canal 1 del Timer 4
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_2);      //Se inicializa el Canal 2 del Timer 4
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_3);      //Se inicializa el Canal 3 del Timer 4
	HAL_TIM_PWM_Start(&htim4,TIM_CHANNEL_4);      //Se inicializa el Canal 4 del Timer 4
	

  while (1) { 
		Tiempo_inicio_mot=HAL_GetTick();	
			TIM4->CCR1= motA1;   //PD12
			TIM4->CCR2= motB1;   //PB7	
			TIM4->CCR3= motB2;	  //PB8
			TIM4->CCR4= motA2;	  //PD15	
		osDelay(10);
		Tiempo_final_mot=HAL_GetTick();  
		Tiempo_dif_mot = Tiempo_final_mot - Tiempo_inicio_mot;
		sprintf (Dato_SD_mot, "Start,%d,End,%d,Dif,%d",Tiempo_inicio_mot,Tiempo_final_mot, Tiempo_dif_mot);		
		ImprimirSD(nomTXT_mot,Dato_SD_mot);
  }
}

void DatoMotores(int der1,int der2,int izq1, int izq2){
	motA1=der1;
	motA2=der2;
	motB1=izq1;
	motB2=izq2;	
}
