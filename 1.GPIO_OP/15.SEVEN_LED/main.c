#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
int i=0,j=0;
int delay_ms(void);
//------------------------------------------------------------------------------
// Function Name : Init _GPIO pins 
// Description : pins ,port clock & mode initialization.
//------------------------------------------------------------------------------



/*******************************************************************************
* Function Name : main
* Description : Main program.
*******************************************************************************/
int main(void)
{

	/* GPIO config */
GPIO_InitTypeDef GPIO_InitStructure;
/* GPIOA GPIOB clock enable */
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);

/* Configure 8 ,9 PINS as input floating */
GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;
/* Configure PORT clock speed */
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
/* Configure PORT mode as input */
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
/*configure PINS to output pushpull mode*/
/* Above all GPIO instructions apply to port "C"*/
GPIO_Init(GPIOC, &GPIO_InitStructure);

/* Configure 8,9,10,11,12,13 pins as input floating */
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;
;
/* Configure PORT clock speed */
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
/* Configure PORT mode as input */
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
/*configure PINS to output pushpull mode*/ 

/* Above all GPIO instructions apply to port "B"*/
GPIO_Init(GPIOA, &GPIO_InitStructure);
while(1)//infinite loop
	{ 
/*ODR bits can be individually set and reset */
//for displaying 0
GPIOA->ODR = 0x0007; 
GPIOC->ODR = 0x0007;
delay_ms(); 
//for displaying 1
GPIOA->ODR = 0x0001; 
GPIOC->ODR = 0x0004;
delay_ms(); 
//for displaying 2
GPIOA->ODR = 0x000B; 
GPIOC->ODR = 0x0003;
delay_ms(); 
//for displaying 3
GPIOA->ODR = 0x000B; 
GPIOC->ODR = 0x0006;
delay_ms(); 
//for displaying 4
GPIOA->ODR = 0x000D; 
GPIOC->ODR = 0x0004;
delay_ms(); 
//for displaying 5
GPIOA->ODR = 0x000E; 
GPIOC->ODR = 0x0006;
delay_ms(); 
//for displaying 6
GPIOA->ODR = 0x000E; 
GPIOC->ODR = 0x0007;
delay_ms(); 
//for displaying 7
GPIOA->ODR = 0x0003; 
GPIOC->ODR = 0x0004;
delay_ms(); 
//for displaying 8
GPIOA->ODR = 0x000F; 
GPIOC->ODR = 0x0007;
delay_ms(); 
//for displaying 9
GPIOA->ODR = 0x000F; 
GPIOC->ODR = 0x0006;
delay_ms(); 
}
}
int delay_ms(void)
{
	for (i=0;i<1000;i++)
	{
		for (j=0;j<1000;j++)
		{
		}
	}
}