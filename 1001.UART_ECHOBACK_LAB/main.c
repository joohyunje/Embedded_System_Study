#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
	setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
}

#if 0

void Main(void)
{
	Sys_Init(115200);
	// printf("UART2 TX Test\n");
	// printf("\n");
	// A, B, C를 차례로 UART로 출력하는 코드 구현
	while(!Macro_Check_Bit_Set(USART2->SR,7));
	USART2->DR = 'A';
	while(!Macro_Check_Bit_Set(USART2->SR,7));
	USART2->DR = 'B';
	while(!Macro_Check_Bit_Set(USART2->SR,7));
	USART2->DR = 'C';

}

#endif

#if 1

void Main(void)
{
	Sys_Init(115200);
	printf("\nUART2 Echo-Back Test\n");

	for(;;)
	{
		unsigned char x;

		// PC에서 받은 글자를 다시 되돌려 보내는 코드 구현
		while(!Macro_Check_Bit_Set(USART2->SR,5));
		x = USART2->DR;
		while(!Macro_Check_Bit_Set(USART2->SR,7));
		USART2->DR = x;

		
	}
}

#endif
