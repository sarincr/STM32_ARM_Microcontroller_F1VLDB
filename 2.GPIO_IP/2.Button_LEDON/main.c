#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

GPIO_InitTypeDef GPIO_InitStructure;


int main()
{
//...
 
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_14|GPIO_Pin_13;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9|GPIO_Pin_8;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
while (1)
  {
				
		if( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_13) )	
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		}	
		else
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_9);
		}		
		if( GPIO_ReadInputDataBit( GPIOA, GPIO_Pin_14) )	
		{
			GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		}	
		else
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_8);
		}		
															
  }		
																								//wait 250ms	
}	








