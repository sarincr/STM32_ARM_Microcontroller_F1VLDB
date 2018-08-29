#include "stm32f10x.h"
#include "RTE_Components.h"             // Component selection
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC
#include "RTE_Device.h"                 // Keil::Device:Startup
int i=0,j=0;
int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/* ..................................................*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);    
	
	while (1) 
		{
    GPIOC->ODR ^= GPIO_Pin_8;
			for(i=0;i<1000;i++)
			{
				for(j=0;j<1000;j++);
			}

	/* GPIOC Periph clock enable */
	   GPIOB->ODR ^= GPIO_Pin_2;
			for(i=0;i<1000;i++)
			{
				for(j=0;j<1000;j++);
			}

    }
}
