
#include "Thread_rf.h" 
#include "Thread_motores.h"
#include "Thread_lcd.h"
#include "Thread_SD.h"
#include "cmsis_os.h"      
/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_rf': 
 *---------------------------------------------------------------------------*/
 
osThreadId tid_Thread_rf;                                       //Define el BCP de la tarea
osThreadDef (Thread_rf, osPriorityNormal, 1, 0);                 // Define la prioridad de la tarea

int Init_Thread_rf (void) {

  tid_Thread_rf = osThreadCreate (osThread(Thread_rf), NULL);            //Se inicializa la tarea
  if (!tid_Thread_rf) return(-1);
  
  return(0);
}

void Thread_rf (void const *argument) {         // Definicion de Tarea2
	extern int ctrlRF;
	int Estado=0;
	char nomTXT[]="DatosRf.txt";
	char Dato_SD_rf[40];
	static uint32_t Tiempo_inicio_rf,Tiempo_dif_rf,Tiempo_final_rf;	
	int PrimeraVez=0;
  while (1) {
		Tiempo_inicio_rf=HAL_GetTick();		
	if(ctrlRF==1)
		{
		 if  ( HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_6))   //Adelante
			 {
				PrimeraVez=0; 
			  if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1)) //Adelante y Izquierda
				{
				DatoMotores (300,0,0,50);
				Estado=1;
				}
				else if ( HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)) //Adelante y Derecha
				{
				DatoMotores (50,0,0,300);
				Estado=2;
				}		
			  else
			  {
				DatoMotores (200,0,0,200);//Adelante solo
				Estado=3;
				}
			}
			else if (HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_1) ) // Izquierda
			{
				PrimeraVez=0;
				DatoMotores (200,0,200,0);	
				Estado=4;				
			}
			else if ( HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_3)) //Derecha
			{
				PrimeraVez=0;
				DatoMotores (0,200,0,200);
				Estado=5;
			}
			else if ( HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_4)) //Atras
			{
				PrimeraVez=0;
				DatoMotores (0,200,200,0);
				Estado=6;
			}
			else 
			{
				if (PrimeraVez==0){
					PrimeraVez=1;
					DatoMotores (1000,1000,1000,1000);
					osDelay(20);
					DatoMotores (0,0,0,0);
					Estado=7;
				}
			}		
		EstadoMot(Estado);			
		}
		osDelay(5);
		Tiempo_final_rf=HAL_GetTick();  
		Tiempo_dif_rf = Tiempo_final_rf - Tiempo_inicio_rf;
		sprintf (Dato_SD_rf, "Start,%d,End,%d,Dif,%d",Tiempo_inicio_rf,Tiempo_final_rf, Tiempo_dif_rf);		
		ImprimirSD(nomTXT,Dato_SD_rf);		
	}
}
