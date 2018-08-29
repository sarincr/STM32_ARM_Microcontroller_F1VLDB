#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#define SetTIM3Duty( val ) TIM3->CCR3 = val
int main(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;

    volatile int i;
  int n = 1;
    int brightness = 0;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE );

    RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );

    GPIO_StructInit(&GPIO_InitStructure); // Reset init structure

    // Setup Blue & Green LED on STM32-Discovery Board to use PWM.
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;           
	// Alt Function - Push Pull
    GPIO_Init( GPIOC, &GPIO_InitStructure );
    GPIO_PinRemapConfig( GPIO_FullRemap_TIM3, ENABLE ); // Map TIM3_CH3 to GPIOC.Pin8, TIM3_CH4 to GPIOC.Pin9
	// Let PWM frequency equal 100Hz.
  // Let period equal 1000. Therefore, timer runs from zero to 1000. Gives 0.1Hz resolution.
  // Solving for prescaler gives 240.
    TIM_TimeBaseStructInit( &TIM_TimeBaseInitStruct );
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV4;
    TIM_TimeBaseInitStruct.TIM_Period = 1000 - 1;   // 0..999
    TIM_TimeBaseInitStruct.TIM_Prescaler = 240 - 1; // Div 240
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStruct );

    TIM_OCStructInit( &TIM_OCInitStruct );
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    // Initial duty cycle equals 0%. Value can range from zero to 1000.
    TIM_OCInitStruct.TIM_Pulse = 0; // 0 .. 1000 (0=Always Off, 1000=Always On)

    TIM_OC3Init( TIM3, &TIM_OCInitStruct ); // Channel 3 Blue LED
    TIM_OC4Init( TIM3, &TIM_OCInitStruct ); // Channel 4 Green LED

    TIM_Cmd( TIM3, ENABLE );

  while(1)  // Do not exit
  {
    if (((brightness + n) >= 1000) || ((brightness + n) <= 0))
      n = -n; // if  brightness maximum/maximum change direction

        brightness += n;

    TIM3->CCR3 = brightness; // set brightness
    TIM3->CCR4 = 1000 - brightness; // set brightness

    for(i=0;i<10000;i++);  // delay
  }

  return(0); // System will implode
}
