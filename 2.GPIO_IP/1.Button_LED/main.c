#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

GPIO_InitTypeDef GPIO_InitStructure;
int i,j;
#define LED_PORT GPIOC

int main()
{
//...
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
while (1)
  {
		 int inval = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_2);	
		
		/* Write the input value to PD8 */
		if (inval == 0)												//inverse logic due to external pullup resistor
			GPIO_SetBits(GPIOC, GPIO_Pin_9);		//PD8 = 1				
		else
			GPIO_ResetBits(GPIOC, GPIO_Pin_9);	//PD8 = 0																	
  }		
																								//wait 250ms	
}	
