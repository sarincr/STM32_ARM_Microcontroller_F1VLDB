#include "stm32f10x.h"
#include "stdio.h"
 char str[256];
volatile uint32_t delay = 0;

void USART1_Send_Sting(char *string){
    while(*string){
        USART_SendData(USART1, (unsigned short int) *string++);

        while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    }
}


void Init_WU()
{
  GPIO_InitTypeDef GPIO_InitStruct;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void Init_UART(){
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef interrup;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_Init(GPIOA, &GPIO_InitStructure);


    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);

    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);

   interrup.NVIC_IRQChannel = USART1_IRQn;
   interrup.NVIC_IRQChannelPreemptionPriority = 0;
   interrup.NVIC_IRQChannelSubPriority = 0;
   interrup.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&interrup);

   NVIC_EnableIRQ(USART1_IRQn);
}

void Init_BT(void){
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef interrup;

    RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB1Periph_USART2, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;

    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_Init(GPIOA, &GPIO_InitStructure);



    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    USART_Init(USART2, &USART_InitStructure);

    USART_Cmd(USART2, ENABLE);

    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

   interrup.NVIC_IRQChannel = USART2_IRQn;
   interrup.NVIC_IRQChannelPreemptionPriority = 0;
   interrup.NVIC_IRQChannelSubPriority = 0;
   interrup.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&interrup);

   NVIC_EnableIRQ(USART2_IRQn);
}

void BT_Send_String(volatile char *string)
{
   while(*string){
      USART_SendData(USART2,*string);
      ++string;
      while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET){}
   }
}

void PWR_StandbyMode( void )
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  PWR_WakeUpPinCmd(ENABLE);
  PWR_BackupAccessCmd(ENABLE);
  PWR_ClearFlag(PWR_FLAG_SB);
  PWR_EnterSTANDBYMode();
}

void Init_Default_Led(void)
{
   GPIO_InitTypeDef led;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);


   led.GPIO_Pin = GPIO_Pin_9;
   led.GPIO_Mode = GPIO_Mode_Out_PP;
   led.GPIO_Speed = GPIO_Speed_50MHz;

   GPIO_Init(GPIOC,&led);

}

void Delay(volatile uint32_t n)
{
   delay = n;
   while(delay != 0);
}


int main(){
    Init_UART();
    Init_BT();
    Init_Default_Led();
    Init_WU();

    SystemInit();
    if(SysTick_Config(SystemCoreClock / 1000)){
       while(1);
    }

   
    sprintf(str,"\n\nFlash Size : %dKb\nUID : %X%X%X\n",*(volatile u16*)(0x1FFFF7E0),
   *(volatile u32*)(0x1FFFF7E8),*(volatile u32*)(0x1FFFF7EC),*(volatile u32*)(0x1FFFF7F0));
    USART1_Send_Sting(str);
    BT_Send_String(str);

    while(1){
       GPIO_SetBits(GPIOC,GPIO_Pin_9);
       Delay(1000);
       GPIO_ResetBits(GPIOC,GPIO_Pin_9);

       PWR_StandbyMode();
    }
}

void USART1_IRQHandler(void)
{
   if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){

      char c = USART1->DR/*USART_ReciveData(USART1)*/;

      USART_SendData(USART1,c);
   }
}

void USART2_IRQHandler(void)
{
   if(USART_GetITStatus(USART2,USART_IT_RXNE) != RESET){

      char c = USART2->DR/*USART_ReciveData(USART2)*/;

      USART_SendData(USART2,c);
   }
}

void SysTick_Handler(void)
{
   if(delay != 0){
      delay--;
   }
}