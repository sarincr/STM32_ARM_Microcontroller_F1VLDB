#include "stm32f10x.h"
//#include "stdio.h"

#define period 7999   /*Counting number in counter*/
#define prescale 8999 /*Prescale value of the clock*/
/**
BlinkInit is a function to initialize the port PB5. Port mode is push-pull output.
**/
	TIM_TimeBaseInitTypeDef TIMEX;	
void BlinkInit()
{
	GPIO_InitTypeDef GPIODX;					/*Define a GPIO configuration variable*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE); 	/*clock of GPIOC enable*/
	GPIODX.GPIO_Pin = GPIO_Pin_8;				/* pin 5*/
	GPIODX.GPIO_Mode = GPIO_Mode_Out_PP;			/*push-pull output mode*/
	GPIODX.GPIO_Speed = GPIO_Speed_50MHz;			/*GOIO Output frequency*/
	GPIO_Init(GPIOC, &GPIODX);
}

void Delay(uint32_t times){
	for(;times>0;times--);
}

/**
*TIM2_Configuration is to configure TIM2 as a counting clock to generate time interrrupts.
**/
void TIM2_Configuration(){
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); /*Enable the clock of TIM2*/

	TIM_DeInit(TIM2);			/*reset TIM2*/
/**
	output frequency is sysclok/((period+1)*(prescale+1))
**/

	TIMEX.TIM_Period =period;  
	TIMEX.TIM_Prescaler =prescale; 	
	TIMEX.TIM_ClockDivision = TIM_CKD_DIV1;	/* source clock without division*/
	TIMEX.TIM_CounterMode = TIM_CounterMode_Up;/*count up*/	
	TIMEX.TIM_RepetitionCounter = 0; /*update every time when counter overflows*/
	TIM_TimeBaseInit(TIM2,&TIMEX);	/*initialize interrupt*/
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);	/*clear TIM2 interrupt flag*/
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_Trigger,ENABLE); /*8enable TIM2 interrupt*/
	TIM_Cmd(TIM2,ENABLE);			/*TIM2 switch on*/
}


/**
*NVIC_Configuration is a function to configure interrupt vector control, so that interrupt function can match it.
**/
void NVIC_Configuration(){  
	NVIC_InitTypeDef NVIC_InitStructure;			/*Define a NVIC configuration variable*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	  /*there is only one interrupt, so choose group 0*/	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;   /*channel TIM2 enable*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; /*Preemption priority*/
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	/*Sub priority*/
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
  NVIC_Init(&NVIC_InitStructure);
}
/**
* TIM2_IRQHandler is the interrupt function which will execute once when each internal time interrupt occurs.
**/
void TIM2_IRQHandler(void){
	static u8 state=1; 		/*led state, 0 is off, 1 is on*/
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET){
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
			if(state==1){	/* the previous state is on*/
				GPIO_SetBits(GPIOC,GPIO_Pin_8);	/*set PB5 high, led off*/	
				state=0;
			}
			else{	/* the precvious state is off*/
				GPIO_ResetBits(GPIOC,GPIO_Pin_8);/*set PB5 low, led on*/
				state=1;	
			}			
	} /*interrupt occurs*/
}

int main(void)
	{
	SystemInit();
	BlinkInit();
	NVIC_Configuration();
	TIM2_Configuration();
/*	while(1){
		GPIO_SetBits(GPIOC,GPIO_Pin_8);	//set PB5 high, led off
		Delay(7000000);
		GPIO_ResetBits(GPIOC,GPIO_Pin_8);//set PB5 low, led on
		Delay(7000000);	
	}
*/
	while(1);
}
