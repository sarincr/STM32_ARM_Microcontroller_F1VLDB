#include "stm32f10x.h"

void delay(uint32_t delay_count)
{
	while (delay_count) delay_count--;
}
void init_led()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/* GPIOC Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	/* Configure PA0 and PA1 in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
void gpio_toggle(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	GPIOx->ODR ^= GPIO_Pin;
}
int main(void)
{
	init_led();

	while (1) 
		{

		gpio_toggle(GPIOC, GPIO_Pin_8);
		delay(500000);
		delay(500000);
		delay(500000);
		delay(500000);	
		gpio_toggle(GPIOC, GPIO_Pin_9);
		delay(500000);
		delay(500000);
		delay(500000);
		delay(500000);
   	}
}