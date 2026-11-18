#include "device_driver.h"

#define KEY   7

void Key_Poll_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 2); 
	Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, KEY*2);
	Macro_Write_Block(GPIOC->PUPDR,0x3,0x1,KEY*2);
}

/* 0: 디버깅용 설정 */
/* 1: 정상 동작용 설정 */

#if 1
	#define N 		20000
#else
	#define N 		3000000
#endif

static int Key_Check_Input(void)
{
	return Macro_Check_Bit_Clear(GPIOC->IDR, KEY);	
}

int Key_Get_Pressed(void)
{
	unsigned int i, k;

	for(;;)
	{
		k = Key_Check_Input();

		for(i = 0; i < N; i++)
		{
			if(k != Key_Check_Input())
			{
				break;
			}
		}

		if(i == N) break;
	}

	return k;
}

void Key_Wait_Key_Pressed(void)
{
	while(!Macro_Check_Bit_Clear(GPIOC->IDR, KEY));
}

void Key_Wait_Key_Released(void)
{
	while(!Macro_Check_Bit_Set(GPIOC->IDR, KEY));
}

void Key_ISR_Enable(int en)
{
	if(en)
	{
		Macro_Set_Bit(RCC->AHB1ENR, 2); 
		Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 14);

		// SYSCFG 장치 Clock On
		Macro_Set_Bit(RCC->APB2ENR, 2);

		// PC7을 EXTI 7의 소스가 되도록 설정  
		Macro_Write_Block(SYSCFG->EXTICR[1], 0xF, 0x2, 12);
		
		// EXTI 7을 Falling Edge Trigger로 설정   
		EXTI->FTSR |= (1 << 7);


		// EXTI 7을 Rising Edge Trigger로 설정   
		EXTI->RTSR |= (1 << 7);


		// EXTI 7 Pending Clear - 비트연산 금지  
		EXTI->PR = (1 << 7);

		// NVIC EXTI9_5 Interrupt Pending Clear  
		NVIC_ClearPendingIRQ((IRQn_Type)23);

		// EXTI 7 Interrupt Enable  
		Macro_Set_Bit(EXTI->IMR, 7);

		// NVIC EXTI9_5 Interrupt Enable 
		NVIC_EnableIRQ((IRQn_Type)23);
	}

	else
	{
		NVIC_DisableIRQ((IRQn_Type)23);
	}
}