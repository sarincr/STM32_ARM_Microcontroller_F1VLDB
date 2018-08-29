#include "stm32f10x.h"                  // Device header
#include "RTE_Components.h"             // Component selection
#include "RTE_Device.h"                 // Keil::Device:Startup
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO
#include "stm32f10x_rcc.h"              // Keil::Device:StdPeriph Drivers:RCC

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
    GPIO_WriteBit(LED_PORT,GPIO_Pin_8,Bit_SET);
    for (i=0;i<1000000;i++);
    GPIO_WriteBit(LED_PORT,GPIO_Pin_8,Bit_RESET);
    for (i=0;i<1000000;i++);   
  }
}
