/*#############################################################
Description     : Reads the value of 8 switches and displays 
                  this value on 8 LEDs            

            
Circuit         : * 8 LEDs connected to PD15 - PD8
									* 8 switches connected to PE15 - PE8 with
                    pullup resitors


##############################################################*/

/* #includes -------------------------------------------------*/
#include "stm32f10x.h"                  // Device header
																//library for the defaults of a blank project in Keil uVision

/* #defines --------------------------------------------------*/

/* #function prototypes --------------------------------------*/

/* #global variables -----------------------------------------*/

int main(void)
{
	GPIO_InitTypeDef        GPIO_InitStructure;										//structure used for setting up a GPIO port
	RCC_ClocksTypeDef 			RCC_Clocks;														//structure used for setting up the SysTick Interrupt
	
	uint16_t inval;
	
	/* GPIOD and GPIOE Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  /* Configure PD15 - PD8 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | 
																GPIO_Pin_13 | GPIO_Pin_12 | 
																GPIO_Pin_11 | GPIO_Pin_10 | 
																GPIO_Pin_9 | GPIO_Pin_8;	//which pins to setup, seperated by |
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;					//output not pulled up or down
  GPIO_Init(GPIOC, &GPIO_InitStructure);												//initialize GPIOD with the above structure
  
	/* Configure PE15 - PE8 in input mode */
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  /* Configure PD15 - PD8 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 | GPIO_Pin_14 | 
																GPIO_Pin_13 | GPIO_Pin_12 | 
																GPIO_Pin_11 | GPIO_Pin_10 | 
																GPIO_Pin_9 | GPIO_Pin_8;	//which pins to setup, seperated by |
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;					//output not pulled up or down
  GPIO_Init(GPIOA, &GPIO_InitStructure);							//initialize GPIOE with the above structure
	
	/* Set the SysTick Interrupt to occur every 1ms) */
	RCC_GetClocksFreq(&RCC_Clocks);
	
	if (SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000))
		while(1);																										//will end up in this infinite loop if there was an error with Systick_Config
	
	/* Main program loop */
  while (1)
  {
		/* Read the input port and store the value in inval, then shift the result 8 spaces
		to the right so that it represents a value between 0 and 255.  E.g. if all the switches were
		OFF, then the inval = 0xff00 (inverse logic due to pullup resistors), after the 
		shift it is 0x00ff */
		inval = GPIO_ReadInputData(GPIOA) >> 8;		
		
		/* Write the input value to PD15 - PD8 */
		GPIO_Write(GPIOC, inval << 8);						//shift the inval bits 8 spaces to right to be written to PD15 - PD8																	
  }		
}

