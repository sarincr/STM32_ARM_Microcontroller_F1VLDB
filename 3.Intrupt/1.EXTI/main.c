#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_exti.h"
#include "misc.h"
int i=0,j=0;
void LED_Init(void)
{
        GPIO_InitTypeDef GpioLedInit;
	      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);
        //enable clocks to GPIOD
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
        GpioLedInit.GPIO_Mode=GPIO_Mode_Out_PP;
        GpioLedInit.GPIO_Pin=GPIO_Pin_8;
        GpioLedInit.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOC,&GpioLedInit);
}
void ButtonInit(void)
{
  //EXTI structure to init EXT
  EXTI_InitTypeDef EXTGM;
  //NVIC structure to set up NVIC controller
  NVIC_InitTypeDef Vector;
  //Connect EXTI Line to Button Pin
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource0);
  //Configure Button EXTI line
  EXTGM.EXTI_Line = EXTI_Line0;
  //select interrupt mode
  EXTGM.EXTI_Mode = EXTI_Mode_Interrupt;
  //generate interrupt on rising edge
  EXTGM.EXTI_Trigger = EXTI_Trigger_Falling;
  //enable EXTI line
  EXTGM.EXTI_LineCmd = ENABLE;
  //send values to registers
  EXTI_Init(&EXTGM);
  //configure NVIC
  //select NVIC channel to configure
  Vector.NVIC_IRQChannel = EXTI0_IRQn;
  //set priority to lowest
  Vector.NVIC_IRQChannelPreemptionPriority = 1;
  //set subpriority to lowest
  Vector.NVIC_IRQChannelSubPriority = 1;
  //enable IRQ channel
  Vector.NVIC_IRQChannelCmd = ENABLE;
  //update NVIC registers
  NVIC_Init(&Vector);
}
int main(void)
{
	ButtonInit();
	  LED_Init();
	  while(1)
    {
			for(i=0;i<10000;i++)
			{
      for(j=0;j<10000;j++)
				{
				}
			}
		  GPIO_WriteBit(GPIOC,GPIO_Pin_8,RESET);
    }
}
void EXTI0_IRQHandler(void)
{
    //Check if EXTI_Line0 is asserted
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
   //clear interrupt and start counting again
    	EXTI_ClearITPendingBit(EXTI_Line0);
      GPIO_WriteBit(GPIOC,GPIO_Pin_8,SET);
 
    }
    //we need to clear line pending bit manually

}
