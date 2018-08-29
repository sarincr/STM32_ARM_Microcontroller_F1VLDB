#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"

int main()
{
	      GPIO_InitTypeDef GpioLedInit;
	      NVIC_InitTypeDef NVIC_InitStructure;
	      TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
        
        //enable clocks to GPIOC
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
        GpioLedInit.GPIO_Mode=GPIO_Mode_Out_PP;
        GpioLedInit.GPIO_Pin=GPIO_Pin_8;
        GpioLedInit.GPIO_Speed=GPIO_Speed_50MHz;
        GPIO_Init(GPIOC,&GpioLedInit);


        //enable clocks to tim2
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

       
        //enable tim2 IRQ
        NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);


        //setting timer 2 interrupt

        
        TIM_TimeBaseStructure.TIM_Prescaler = 24000 ;
        TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
        TIM_TimeBaseStructure.TIM_Period = 1000-1;
        TIM_TimeBaseStructure.TIM_ClockDivision = 0;
        TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
        /* TIM IT enable */
        TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

        /* TIM2 enable counter */
        TIM_Cmd(TIM2, ENABLE);
				while(1)
        {

        }
}

void TIM2_IRQHandler(void)
{
        //if interrupt happens the do this
        if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
        {
                //clear interrupt and start counting again
                TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
                //toggle led
                if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_8) == RESET)
                {
                        GPIO_SetBits(GPIOC,GPIO_Pin_8);
                }else
                {
                        GPIO_ResetBits(GPIOC,GPIO_Pin_8);
                }
        }
				
     
}
