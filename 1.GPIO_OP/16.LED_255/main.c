#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x.h"                  // Device header

GPIO_InitTypeDef GPIO_InitStructure;
int i,count;
#define LED_PORT GPIOC

int main()
{
//...
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8;	 ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
		
while (1)
  {
		GPIO_Write(GPIOC, count << 8);//display count on LEDs, use bits 15 to 8, therfore we had to shift the count bits to the left 8 spaces
    count = count + 1;            //increment count; because count is a 8-bit unsigned value, the count will "roll-over" from 255 back to 0 again
		for (i=0;i<1000000;i++)
		{
		}
    for (i=0;i<1000000;i++)
		{
		}	
  }		
																								
}	








