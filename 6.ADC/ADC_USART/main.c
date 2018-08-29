#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
void GPIO_Configuration(void);
void USART_Configuration(void);
void ADC_Configuration(void);
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
GPIO_InitTypeDef GPIO_InitStructure;
ADC_InitTypeDef ADC_InitStructure;

int   adval;
char buf[10];

/* Blink a LED, blink speed is set by ADC value */
int main()
{
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	 GPIO_Configuration();
	 USART_Configuration();
// input of ADC (it doesn't seem to be needed, as default GPIO state is floating input)
	
	 
	    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
        GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1 ;            // that's ADC1 (PA1 on STM32)
        GPIO_Init(GPIOA, &GPIO_InitStructure);

//clock for ADC (max 14MHz --> 72/6=12MHz)
        RCC_ADCCLKConfig (RCC_PCLK2_Div6);
// enable ADC system clock
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

// define ADC config
        ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
        ADC_InitStructure.ADC_ScanConvMode = DISABLE;
        ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;      // we work in continuous sampling mode
        ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
        ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
        ADC_InitStructure.ADC_NbrOfChannel = 1;

        ADC_RegularChannelConfig(ADC1,ADC_Channel_1, 1,ADC_SampleTime_28Cycles5); // define regular conversion config
        ADC_Init ( ADC1, &ADC_InitStructure);   //set config of ADC1

// enable ADC
        ADC_Cmd (ADC1,ENABLE);  //enable ADC1

//      ADC calibration (optional, but recommended at power on)
        ADC_ResetCalibration(ADC1);     // Reset previous calibration
        while(ADC_GetResetCalibrationStatus(ADC1));
        ADC_StartCalibration(ADC1);     // Start new calibration (ADC must be off at that time)
        while(ADC_GetCalibrationStatus(ADC1));

// start conversion
        ADC_Cmd (ADC1,ENABLE);  //enable ADC1
        ADC_SoftwareStartConvCmd(ADC1, ENABLE); // start conversion (will be endless as we are in continuous mode)



        while(1)
        {
        	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
        	adval=ADC_GetConversionValue(ADC1);
        	sprintf((char *)buf,"ADC=%4d",(int)adval);
        	UARTSend(buf,sizeof(buf));
        	UARTSend("\r\n",2);
        }
}
void USART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

/* USART1 configuration ------------------------------------------------------*/
  /* USART1 configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the middle
        - USART LastBit: The clock pulse of the last data bit is not output to
                         the SCLK pin
  */
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  USART_Init(USART1, &USART_InitStructure);

  /* Enable USART1 */
  USART_Cmd(USART1, ENABLE);
}
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    //
    // Loop while there are more characters to send.
    //
    while(ulCount--)
    {
        USART_SendData(USART1, *pucBuffer++);// Last Version USART_SendData(USART1,(uint16_t) *pucBuffer++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        }
    }

}
void GPIO_Configuration(void)
{

  

  /* Configure USART1 Tx (PA.09) as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART1 Rx (PA.10) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
