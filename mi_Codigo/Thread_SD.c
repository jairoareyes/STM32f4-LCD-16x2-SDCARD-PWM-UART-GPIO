
#include "cmsis_os.h"                                           // CMSIS RTOS header file
#include "stm32f4xx_hal.h"
#include "fatfs.h"
#include "Thread_lcd.h"
#include "Thread_SD.h"
#include <string.h>

FATFS FatFs;
FIL Fil;




/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
osThreadId tid_Thread_SD;                                          // thread id
osThreadDef (Thread_SD, osPriorityNormal, 1, 0);                   // thread object
char Dato_SD_imp[40];
char nomTXT_imp[14];

osMutexDef (sd_mutex);    // Declare mutex
osMutexId  (sd_mutex_id); // Mutex ID


int Init_Thread_SD (void) {

  tid_Thread_SD = osThreadCreate (osThread(Thread_SD), NULL);
  if (!tid_Thread_SD) return(-1);
  
  return(0);
}

void Thread_SD (void const *argument) {

	sd_mutex_id = osMutexCreate(osMutex(sd_mutex));
	char Dato_SD_sd[40];
	char nomTXT_sd[]="DatosSD.txt";
	static uint32_t Tiempo_inicio_sd,Tiempo_dif_sd, Tiempo_final_sd;
	
  while (1)
	{
		Tiempo_inicio_sd=HAL_GetTick();
		osDelay(100);
		SDdatos(nomTXT_imp,Dato_SD_imp);
		osDelay(100);
		Tiempo_final_sd=HAL_GetTick();  
		Tiempo_dif_sd = Tiempo_final_sd - Tiempo_inicio_sd;
		sprintf (Dato_SD_sd, "Start,%d,End,%d,Dif,%d",Tiempo_inicio_sd,Tiempo_final_sd, Tiempo_dif_sd);
		SDdatos(nomTXT_sd,Dato_SD_sd);
	
	}	
 }
void ImprimirSD(char nomArch1[],char datosSD1[]){
		strcpy(nomTXT_imp, nomArch1);
		strcpy(Dato_SD_imp, datosSD1);
	}
void SDdatos(char nomArch[],char datosSD[]){
		osMutexWait(sd_mutex_id, osWaitForever);
    HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
    if (f_mount(&FatFs, "", 1) == FR_OK) 
			{
        //Try to open file
        if (f_open(&Fil, nomArch, FA_OPEN_ALWAYS | FA_READ | FA_WRITE) == FR_OK)   //Datos.csv
					{  
					  f_lseek (&Fil,f_size(&Fil));
            HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
            //Toma de datos para todas las tareas del proyecto
            
					 if (((f_puts(datosSD, &Fil))&(f_puts("\n\r", &Fil)))>0) 
					  {
             HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_SET);
            }
            f_close(&Fil);
						HAL_GPIO_WritePin(GPIOD,GPIO_PIN_14,GPIO_PIN_RESET);
					 }				
        }
        f_mount(0, "", 1);
  osMutexRelease(sd_mutex_id);			
}
