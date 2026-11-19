#include "device_driver.h"

#define LED1   7
#define LED2   6

void LED_Init(void)
{
	Macro_Set_Bit(RCC->AHB1ENR, 0);

	Macro_Write_Block(GPIOA->MODER, 0x3, 0x1, 10);
	Macro_Clear_Bit(GPIOA->OTYPER, 5);
	Macro_Clear_Bit(GPIOA->ODR, 5);

	Macro_Write_Block(GPIOA->MODER, 0x3, 0x1, LED1 * 2);
	Macro_Clear_Bit(GPIOA->OTYPER, LED1);
	Macro_Clear_Bit(GPIOA->ODR, LED1);

	Macro_Write_Block(GPIOA->MODER, 0x3, 0x1, LED2 * 2);
	Macro_Clear_Bit(GPIOA->OTYPER, LED2);
	Macro_Clear_Bit(GPIOA->ODR, LED2);

}

void LED_state(int led_s)
{
    if(led_s == 1)
	{
		Macro_Set_Bit(GPIOA->ODR, LED1);
		Macro_Clear_Bit(GPIOA->ODR, LED2);

	}
	else if(led_s == 2)
	{
		Macro_Set_Bit(GPIOA->ODR, LED2);
		Macro_Clear_Bit(GPIOA->ODR, LED1);
	}
	else{
		Macro_Clear_Bit(GPIOA->ODR, LED2);
		Macro_Clear_Bit(GPIOA->ODR, LED1);
	}

}
