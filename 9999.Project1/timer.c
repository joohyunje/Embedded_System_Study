#include "device_driver.h"

#define TIM4_TICK	  		(40)
#define TIM4_FREQ 	  		(1000000./TIM4_TICK)
#define TIME4_PLS_OF_1ms  	(1000./TIM4_TICK)

void TIM4_Repeat_Interrupt_Enable(int en, int time)
{
	if(en)
	{
		Macro_Set_Bit(RCC->APB1ENR,2);

		TIM4->CR1 = (0<<4)|(1<<3);
		TIM4->PSC = (unsigned int)(TIMXCLK/(double)TIM4_FREQ + 0.5)-1;
		TIM4->ARR = TIME4_PLS_OF_1ms * time;
		Macro_Set_Bit(TIM4->EGR,0);

		Macro_Clear_Bit(TIM4->SR,0);
		NVIC_ClearPendingIRQ(30);
		Macro_Set_Bit(TIM4->DIER,0);
		NVIC_EnableIRQ(30);

		Macro_Set_Bit(TIM4->CR1,0);
	}

	else
	{
		NVIC_DisableIRQ(30);
		Macro_Clear_Bit(TIM4->CR1, 0);
		Macro_Clear_Bit(TIM4->DIER, 0);
	}
}
