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

static void Motor_Init(){

	Macro_Write_Block(GPIOA->MODER, 0xf, 0x5, 0);
	Macro_Clear_Bit(GPIOA->OTYPER, 0);
	Macro_Clear_Bit(GPIOA->OTYPER, 1);
	Macro_Clear_Bit(GPIOA->ODR, 0);
	Macro_Clear_Bit(GPIOA->ODR, 1);

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
	Motor_Init();
	Key_Poll_Init();

	printf("\nProject1\n");

	Macro_Set_Bit(GPIOA->ODR,1);
	Macro_Clear_Bit(GPIOA->ODR,0);
	
	int flag =0;
	
	for(;;)
	{
		//toggling 
		if((flag ==0)&&Key_Get_Pressed()){

			Macro_Invert_Bit(GPIOA->ODR,5);
			Macro_Invert_Bit(GPIOA->ODR,1);
			for(volatile int i = 0; i < 0x700000; i++);
			Macro_Invert_Bit(GPIOA->ODR,0);
			flag =1;

		}else if((flag ==1)&&!Key_Get_Pressed()){
			
			flag =0;
			
		}
	}
	
	

	
		
	

}

#endif
