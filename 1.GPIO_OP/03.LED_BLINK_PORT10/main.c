#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

GPIO_InitTypeDef GPIO_InitStructure;
int i;
#define LED_PORT GPIOC

int main()
{
//...
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_All ;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  while(1)
		{
   GPIO_WriteBit(LED_PORT,GPIO_Pin_10,Bit_SET);
    for (i=0;i<1000000;i++);
    GPIO_WriteBit(LED_PORT,GPIO_Pin_10,Bit_RESET);
    for (i=0;i<1000000;i++);
		}
}
