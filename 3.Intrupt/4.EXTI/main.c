  /******************************************************************************
  * @file    main.c 
  * @author  Erik Herkenrath
  * @version V1.0.0
  * @date    Apr 11. 2012
  * @brief   hardware init for RC-Servo PWM signal input and output
  *          tested on STM32 value line discovery Board
  *          µC: STM32F100RB @24 MHz
  ******************************************************************************/
#include "stm32f10x.h"                  // Device header

/* RC-receiver input */
__IO int32_t TIM3_IC1 = 0; /* Timer3, Input Compare Channel1, PA6 */
__IO int32_t TIM3_IC2 = 0; /* Timer3, Input Compare Channel2, PA7 */
__IO int32_t TIM3_IC3 = 0; /* Timer3, Input Compare Channel3, PB0 */
__IO int32_t TIM3_IC4 = 0; /* Timer3, Input Compare Channel4, PB1 */

/* Main function -------------------------------------------------------------*/
int main(void)
{
	/*******************/
	/*  Hardware Init  */
	/*******************/

	/* HW init structures */
	GPIO_InitTypeDef  GPIO_InitStructure; /* Structure for port init */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure; /* Structure for basic Timer init */
	TIM_OCInitTypeDef TIM_OCInitStructure; /* Structure for Timer Output Compare init */
	TIM_ICInitTypeDef TIM_ICInitStructure; /* Structure for Timer Input Capture init */
	NVIC_InitTypeDef NVIC_InitStructure; /* NVIC priority grouping */

	/* Enable clocks */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3, ENABLE); /* Enable clocking of Timer Periperials */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE); /* Enable clocking of Ports */

	/* Configure IO-pins */
	/* PWM output */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	/* Push-Pull Output Alternate-Function */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; /* = lowest output impedance */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;	/* PA0: TIM2 CHN1, PA1: TIM2 CHN2 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* PWM input */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; /* Input Floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; /* PA6: TIM3 CHN1, PA7: TIM3 CHN2 */
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; /* PB0: TIM3 CHN3, PB1: TIM3 CHN4 */
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Initialize timer2 - output compare PWM */
//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE); /* Remap Pins if necessary */
	TIM_TimeBaseStructure.TIM_Period = 20000 - 1; /* PWM signal length 20 ms */
	TIM_TimeBaseStructure.TIM_Prescaler = 24 - 1; /* counter increments every 1 µs */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); /* initialize timer */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; /* PWM generation */
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; /* enable PWM-output */
	TIM_OC1Init(TIM2, &TIM_OCInitStructure); /* initialize channel1 (PA0) */
	TIM_OC2Init(TIM2, &TIM_OCInitStructure); /* initialize channel2 (PA1) */
	TIM_OC3Init(TIM2, &TIM_OCInitStructure); /* initialize channel3 (PA2) */
	TIM_OC4Init(TIM2, &TIM_OCInitStructure); /* initialize channel4 (PA3) */
	TIM_Cmd(TIM2, ENABLE); /* start timer */

// zur Laufzeit festlegen: RC-Servo Ansteuerung	(hier testweise)
TIM2->CCR1 = 1000; /* set puls length: 1.0 ms = 0 % */
TIM2->CCR2 = 1333; /* set puls length: 1.3 ms = 33 % */
TIM2->CCR3 = 1666; /* set puls length: 1.7 ms = 66 % */
TIM2->CCR4 = 2000; /* set puls length: 2.0 ms = 100 % */

	/* Initialize Timer3 - input capture PWM */
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF; /* overflow after ~65 ms */
	TIM_TimeBaseStructure.TIM_Prescaler = 24 - 1; /* counter increments every 1 µs */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); /* initialize timer */
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; /* trigger capture event on rising edge */
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; /* input pins directly connected */
	TIM_ICInitStructure.TIM_ICFilter = 0x3;	/* input must be stable for 8 ticks */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; /* configure channel1 (PA6) */
	TIM_ICInit(TIM3, &TIM_ICInitStructure);	/* configure input capture */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; /* configure channel2 (PA7) */
	TIM_ICInit(TIM3, &TIM_ICInitStructure);	/* configure input capture */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_3; /* configure channel3 (PB0) */
	TIM_ICInit(TIM3, &TIM_ICInitStructure);	/* configure input capture */
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_4; /* configure channel4 (PB1) */
	TIM_ICInit(TIM3, &TIM_ICInitStructure);	/* configure input capture */
	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE); /* enable CC-interrupts */
	TIM_Cmd(TIM3, ENABLE); /* start timer */
	
	/* Configure Nested Interrupt Controller (Int-Enable should be the last point in Hardware Init!) */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); /* 1 bit for pre-emption priority, 3 bits for subpriority */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; /* second highest preemption-priority */
	/* Enable the Timer3 Interrupts - input capture */
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; /* highest sub-priority */
	NVIC_Init(&NVIC_InitStructure);

	while(1) {
		
	}

}


  /******************************************************************************
  * @file    stm32f10x_it.c 
  * @author  Erik Herkenrath
  * @version V1.0.0
  * @date    Apr 11. 2012
  * @brief   Interrupt Service Routine for input capture of RC-Servo PWM signal
  ******************************************************************************/

void TIM3_IRQHandler(void)
{
	static uint16_t CCR1_rise;
	static uint16_t CCR2_rise;
	static uint16_t CCR3_rise;
	static uint16_t CCR4_rise;

	if(TIM3->SR & TIM_IT_CC1) {					/* input capture channel1? */
		if(TIM3->CCER & TIM_CCER_CC1P) {		/* captured falling edge? */
			TIM3_IC1 = TIM3->CCR1 - CCR1_rise;	/* Get the Input Capture value, calculate high pulse length */
			TIM3->CCER &= ~TIM_CCER_CC1P;		/* select rising edge as capture event */
		}		
		else {									/* captured rising edge? */
			CCR1_rise = TIM3->CCR1;				/* Get the Input Capture value */
			TIM3->CCER |= TIM_CCER_CC1P;		/* select falling edge as capture event */
		}
		TIM3->SR = ~TIM_IT_CC1;					/* Clear interrupt pending bit */
	}

	if(TIM3->SR & TIM_IT_CC2) {					/* input capture channel2? */
		if(TIM3->CCER & TIM_CCER_CC2P) {		/* captured falling edge? */
			TIM3_IC2 = TIM3->CCR2 - CCR2_rise;	/* Get the Input Capture value, calculate high pulse length */
			TIM3->CCER &= ~TIM_CCER_CC2P;		/* select rising edge as capture event */
		}		
		else {									/* captured rising edge? */
			CCR2_rise = TIM3->CCR2;				/* Get the Input Capture value */
			TIM3->CCER |= TIM_CCER_CC2P;		/* select falling edge as capture event */
		}
		TIM3->SR = ~TIM_IT_CC2;					/* Clear interrupt pending bit */
	}
	
	if(TIM3->SR & TIM_IT_CC3) {					/* input capture channel3? */
		if(TIM3->CCER & TIM_CCER_CC3P) {		/* captured falling edge? */
			TIM3_IC3 = TIM3->CCR3 - CCR3_rise;	/* Get the Input Capture value, calculate high pulse length */
			TIM3->CCER &= ~TIM_CCER_CC3P;		/* select rising edge as capture event */
		}		
		else {									/* captured rising edge? */
			CCR3_rise = TIM3->CCR3;				/* Get the Input Capture value */
			TIM3->CCER |= TIM_CCER_CC3P;		/* select falling edge as capture event */
		}
		TIM3->SR = ~TIM_IT_CC3;					/* Clear interrupt pending bit */
	}

	if(TIM3->SR & TIM_IT_CC4) {					/* input capture channel4? */
		if(TIM3->CCER & TIM_CCER_CC4P) {		/* captured falling edge? */
			TIM3_IC4 = TIM3->CCR4 - CCR4_rise;	/* Get the Input Capture value, calculate high pulse length */
			TIM3->CCER &= ~TIM_CCER_CC4P;		/* select rising edge as capture event */
		}		
		else {									/* captured rising edge? */
			CCR4_rise = TIM3->CCR4;				/* Get the Input Capture value */
			TIM3->CCER |= TIM_CCER_CC4P;		/* select falling edge as capture event */
		}
		TIM3->SR = ~TIM_IT_CC4;					/* Clear interrupt pending bit */
	}
}