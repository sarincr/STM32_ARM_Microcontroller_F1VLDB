#include "stm32f10x.h"
#define GPIO_LED GPIO_Pin_8
#define GPIO_BUT GPIO_Pin_0

int main(void) 
	{
     // initializing a structure
    GPIO_InitTypeDef GPIO_InitStructure;

    // enabling all GPIO clock
 
    

    // setting the GPIO_Speed by default (2MHz)
    GPIO_StructInit(&GPIO_InitStructure);

    // setting the LCD pin
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_LED;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    // setting the USER BUTTON pin
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	  GPIO_InitStructure.GPIO_Pin = GPIO_BUT;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // infinite loop
    while (1337) 
			{
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_BUT)) 
					{
            GPIO_WriteBit(GPIOC, GPIO_LED, Bit_SET);
        } else {
            GPIO_WriteBit(GPIOC, GPIO_LED, Bit_RESET);
        }
    }
}