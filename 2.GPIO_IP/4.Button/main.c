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
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_15 | GPIO_Pin_14 | GPIO_Pin_13 | GPIO_Pin_12 | GPIO_Pin_11 | GPIO_Pin_10 | GPIO_Pin_9 | GPIO_Pin_8;	 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;	 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
		
while (1)
  {
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)					//is the pushbutton pressed (input = 0)      
    {
			for (i=0;i<10000;i++)
		{
		}
    		//wait 10ms for debounce
			if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)     	//is the pushbutton still presed? otherwise
			{
				while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
				{
				}	//wait for the pushbutton to be released
        count = count + 1;         														//increment count
        GPIO_Write(GPIOC, count << 8);  											//display count on LEDs
		  }
    } 
  }
}	
