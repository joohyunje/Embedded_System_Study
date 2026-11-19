#include "device_driver.h"

#define KEY   7

void Key_Poll_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 2);
	Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, KEY*2);
	Macro_Write_Block(GPIOC->PUPDR,0x3,0x1,KEY*2);
}

void Key_ISR_Enable(int en)
{
	if(en)
	{
		Macro_Set_Bit(RCC->AHB1ENR, 2);
		Macro_Write_Block(GPIOC->MODER, 0x3, 0x0, 14);

		Macro_Set_Bit(RCC->APB2ENR, 14);

		Macro_Write_Block(SYSCFG->EXTICR[1], 0xF, 0x2, 12);

		EXTI->FTSR |= (1 << 7);

		EXTI->RTSR |= (1 << 7);

		EXTI->PR = (1 << 7);

		NVIC_ClearPendingIRQ((IRQn_Type)23);

		Macro_Set_Bit(EXTI->IMR, 7);

		NVIC_EnableIRQ((IRQn_Type)23);
	}

	else
	{
		NVIC_DisableIRQ((IRQn_Type)23);
	}
}
