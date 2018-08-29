//-------------------------------------------STM32F4_TIMER_INTERRUPT-------------------------------------------------------

/* Beschreibung:
 * Ein Timer erzeugt einen Interrupt und eine LED blinkt
 */
/*

Hinweis zum Clock fuer 8MHz Quarz, damit er auf 168MHz läuft (betrifft das stm32F4Discovery):

in der system_stm32f4xx.c muss eingetragen sein:
PLL_M = 8

in der stm32f4xx.h muss eingetragen sein:
HSE_VALUE = 8000000
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f10x.h"                  // Device header


volatile uint32_t timer_tick;

void GPIO_INIT_EXAMPLE(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  /* GPIOC Periph clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void TIMER_INIT_EXAMPLE(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

	// Der Timer wird mit 84MHz getaktet (APB1 Domain mit 42MHz X2 siehe Seite 85 im Ref.Man.)
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	// Time Base initialisieren
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 24000-1; // 	84MHz / 1000 = 84 kHz nach Prescaler
	TIM_TimeBaseInitStructure.TIM_Period = 1000 - 1;  //	84khz / 8400 = 10 Hz endgültige Updaterate des Timers
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

	// Interrupt enable für Update
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);

	// um den Timer loslaufen zu lassen
	TIM_Cmd(TIM2,ENABLE);
}

void NVIC_INIT_EXAMPLE(uint16_t IRQ)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = IRQ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // Die Priorität --> kleine Zahlen = hohe Priorität
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM2_IRQHandler(void)
{
	while( (TIM2->SR & (1<<0)) > 0 ) TIM2->SR &= ~(1<<0); // clear UIF flag
	timer_tick++;
}


int main(void)
{

	SystemInit();

	TIMER_INIT_EXAMPLE();

	NVIC_INIT_EXAMPLE(TIM2_IRQn);

	GPIO_INIT_EXAMPLE();


	while(1)
    {

		////////////////////////timer_tick////////////////////////
		if (timer_tick >= 5)
		{
			GPIOC->ODR ^= GPIO_Pin_9; // Toggle LED
			timer_tick = 0;
		}
		///////////////////////////////////////////////////////////
    }
}
