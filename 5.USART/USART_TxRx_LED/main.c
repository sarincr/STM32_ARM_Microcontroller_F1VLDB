#include <stm32f10x.h>
#include <misc.h>			 // I recommend you have a look at these in the ST firmware folder
int i,j;
#define MAX_STRLEN 12 // this is the maximum string length of our string in characters
volatile char received_string[MAX_STRLEN+1]; // this will hold the recieved string

void Delay(__IO uint32_t nCount)
 {
  while(nCount--) {
  }
}

/* This funcion initializes the USART1 peripheral
 * 
 * Arguments: baudrate --> the baudrate at which the USART is 
 * 						   supposed to operate
 */
void init_USART1(uint32_t baudrate){
	
	GPIO_InitTypeDef GPIO_InitStruct; // this is for the GPIO pins used as TX and RX
	USART_InitTypeDef USART_InitStruct; // this is for the USART1 initilization
	NVIC_InitTypeDef NVIC_InitStructure; // this is used to configure the NVIC (nested vector interrupt controller)
	
	/* enable APB2 peripheral clock for USART1 
	 * note that only USART1 and USART6 are connected to APB2
	 * the other USARTs are connected to APB1
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	/* enable the peripheral clock for the pins used by 
	 * USART1, PA9 for TX and PA10 for RX
	 */

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; 				// Pin 9 (TX)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP; 		// the pins are configured as alternate function so the USART peripheral has access to them, Push pull output
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_Init(GPIOC, &GPIO_InitStruct);		
	
	/* This sequence sets up the TX and RX pins 
	 * so they work correctly with the USART1 peripheral
	 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9; 				// Pin 9 (TX)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; 		// the pins are configured as alternate function so the USART peripheral has access to them, Push pull output
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;		// this defines the IO speed and has nothing to do with the baudrate!
	GPIO_Init(GPIOA, &GPIO_InitStruct);					// now all the values are passed to the GPIO_Init() function which sets the GPIO registers
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;				// Pin 10 (RX)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;  // Input mode floating
	GPIO_Init(GPIOA, &GPIO_InitStruct);
		
	/* Now the USART_InitStruct is used to define the 
	 * properties of USART1 
	 */
	USART_InitStruct.USART_BaudRate = baudrate;				// the baudrate is set to the value we passed into this init function
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;// we want the data frame size to be 8 bits (standard)
	USART_InitStruct.USART_StopBits = USART_StopBits_1;		// we want 1 stop bit (standard)
	USART_InitStruct.USART_Parity = USART_Parity_No;		// we don't want a parity bit (standard)
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // we don't want flow control (standard)
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; // we want to enable the transmitter and the receiver
	USART_Init(USART1, &USART_InitStruct);					// again all the properties are passed to the USART_Init function which takes care of all the bit setting
	
	
	/* Here the USART1 receive interrupt is enabled
	 * and the interrupt controller is configured 
	 * to jump to the USART1_IRQHandler() function
	 * if the USART1 receive interrupt occurs
	 */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // enable the USART1 receive interrupt 
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;		 // we want to configure the USART1 interrupts
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		 // this sets the subpriority inside the group
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			 // the USART1 interrupts are globally enabled
	NVIC_Init(&NVIC_InitStructure);							 // the properties are passed to the NVIC_Init function which takes care of the low level stuff	
	
	// finally this enables the complete USART1 peripheral
	USART_Cmd(USART1, ENABLE);
}

/* This function is used to transmit a string of characters via 
 * the USART specified in USARTx.
 * 
 * It takes two arguments: USARTx --> can be any of the USARTs e.g. USART1, USART2 etc.
 * 						   (volatile) char *s is the string you want to send
 * 
 * Note: The string has to be passed to the function as a pointer because
 * 		 the compiler doesn't know the 'string' data type. In standard
 * 		 C a string is just an array of characters
 * 
 * Note 2: At the moment it takes a volatile char because the received_string variable
 * 		   declared as volatile char --> otherwise the compiler will spit out warnings
 * */
void USART_puts(USART_TypeDef* USARTx, volatile char *s){

	while(*s)
	{
		// wait until data register is empty
		while( !(USARTx->SR & 0x00000040) ); 
		USART_SendData(USARTx, *s);
		*s++;
	}
}

int main(void) {
  
init_USART1(9600); // initialize USART1 @ 9600 baud
while(1)
{
 USART_puts(USART1, "K\r\n"); // just send a message to indicate that it works
}
 
}

// this is the interrupt request handler (IRQ) for ALL USART1 interrupts
void USART1_IRQHandler(void){
	
	// check if the USART1 receive interrupt flag was set
	if( USART_GetITStatus(USART1, USART_IT_RXNE) ){
		
		static uint8_t cnt = 0; // this counter is used to determine the string length
		char t = USART1->DR; // the character from the USART1 data register is saved in t
		
		// check if the received character is not the LF character (used to determine end of string) 
		// or the if the maximum string length has been been reached 
		if( (t == 'G') && (cnt < MAX_STRLEN) )
			{ 
			received_string[cnt] = t;
			cnt++;
			GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_SET);
     for(i=0;i<100;i++)
				{
					for(j=0;j<10;j++)
					{
					}
				}
		}
		if( (t == 'M') && (cnt < MAX_STRLEN) )
		{ // otherwise reset the character counter and print the received string
			cnt = 0;
			USART_puts(USART1, received_string);
			GPIO_WriteBit(GPIOC,GPIO_Pin_9,Bit_RESET);
		}
	}
}







/*
void setup() 
{
   Serial.begin(9600); 

}

void loop() 
{

      Serial.println("G");
      delay(2000);
      Serial.println("M");
      delay(2000);
   
}
*/