#include "device_driver.h"

#define TIM4_TICK	  		(40)
#define TIM4_FREQ 	  		(1000000./TIM4_TICK)
#define TIME4_PLS_OF_1ms  	(1000./TIM4_TICK)
#define TIM3_FREQ 	  			(8000000.)
#define TIM3_TICK	  			(1000000./TIM3_FREQ)
#define TIME3_PLS_OF_1ms  		(1000./TIM3_TICK)
#define TIM2_TICK         	(20)
#define TIM2_FREQ 	  		(1000000./TIM2_TICK)
#define TIME2_PLS_OF_1ms  	(1000./TIM2_TICK)
#define TIM2_MAX	  		(0xffffffffu)

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

void TIM3_Out_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 1);
	Macro_Set_Bit(RCC->APB1ENR, 1);

	Macro_Write_Block(GPIOB->MODER, 0x3, 0x2, 0);
	Macro_Write_Block(GPIOB->AFR[0], 0xf, 0x2, 0);

	Macro_Write_Block(TIM3->CCMR2,0xff, 0x68, 0);
	TIM3->CCER = (0<<9)|(1<<8);
}

void TIM3_Out_Freq_Generation(unsigned short freq)
{
	TIM3->PSC = (int)(TIMXCLK/TIM3_FREQ+0.5)-1;
	TIM3->ARR = (int)(TIM3_FREQ/freq + 0.5);
	TIM3->CCR3 = TIM3->ARR / 2;
	Macro_Set_Bit(TIM3->EGR,0);
	TIM3->CR1 = (0x1<<4)|(0x0<<3)|(0x1<<0);
}

void TIM3_Out_Stop(void)
{
	Macro_Clear_Bit(TIM3->CR1, 0);
}

void TIM2_Delay(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);

	TIM2->CR1 = (1<<4)|(1<<3);
	TIM2->PSC = (unsigned int)(TIMXCLK/TIM2_FREQ + 0.5)-1;
	TIM2->ARR = TIME2_PLS_OF_1ms * time;

	Macro_Set_Bit(TIM2->EGR,0);
	Macro_Clear_Bit(TIM2->SR, 0);
	Macro_Set_Bit(TIM2->CR1, 0);

	while(Macro_Check_Bit_Clear(TIM2->SR, 0));
	Macro_Clear_Bit(TIM2->CR1, 0);
}