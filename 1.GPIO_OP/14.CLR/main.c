
/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void GPIOA_Init(void);
void Delay(uint32_t nCount);

int main(void)
{
	GPIOA_Init();
	
	while(1) {
	GPIO_SetBits(GPIOC , GPIO_Pin_8);
	Delay(0xFFFFF);
	GPIO_ResetBits(GPIOC , GPIO_Pin_8);
	Delay(0xFFFFF);
	}
}

void GPIOA_Init(void) 
	{
	GPIO_InitTypeDef GPIO_InitStructure; //??????
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //??APB2 Peripheral?clock

	/*???GPIO????*/
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8; //??Pin_0???GPIOA?P0
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; //?????push-pul	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //???50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure); //??
}

void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}