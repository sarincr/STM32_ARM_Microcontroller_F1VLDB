#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

GPIO_InitTypeDef LEDBGOP;
int i;

int main()
{
//...
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  LEDBGOP.GPIO_Speed = GPIO_Speed_50MHz;
  LEDBGOP.GPIO_Mode  = GPIO_Mode_Out_PP;
  LEDBGOP.GPIO_Pin   = GPIO_Pin_All ;
  GPIO_Init(GPIOC, &LEDBGOP);

  while(1)
		{
    GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_SET);
    for (i=0;i<1000000;i++);
    GPIO_WriteBit(GPIOC,GPIO_Pin_8,Bit_RESET);
    for (i=0;i<1000000;i++);

    GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_SET);
    for (i=0;i<1000000;i++);
    GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_RESET);
    for (i=0;i<1000000;i++);

   }
}
