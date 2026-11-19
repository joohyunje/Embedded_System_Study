#include "device_driver.h"
#include <stdio.h>

void _Invalid_ISR(void)
{
	unsigned int r = Macro_Extract_Area(SCB->ICSR, 0x1ff, 0);
	printf("\nInvalid_Exception: %d!\n", r);
	printf("Invalid_ISR: %d!\n", r - 16);
	for(;;);
}

extern volatile int TIM4_Expired;
extern volatile int Key_Pressed;

void TIM4_IRQHandler(void)
{
	Macro_Clear_Bit(TIM4->SR,0);
	NVIC_ClearPendingIRQ(30);

	TIM4_Expired = 1;
}

void EXTI9_5_IRQHandler(void)
{
    static int Key_State = 0;

    if(EXTI->PR & (1 << 7))
    {
        unsigned int i, k;

        for(;;)
        {
            k = Macro_Check_Bit_Clear(GPIOC->IDR, 7);

            for(i = 0; i < 20000; i++)
            {
                if(k != Macro_Check_Bit_Clear(GPIOC->IDR, 7))
                    break;
            }

            if(i == 20000) break;
        }

        if(k && Key_State == 0)
        {
            Key_State = 1;
            TIM4_Expired = 0;

            Macro_Clear_Bit(TIM4->CR1, 0);
            TIM4->CNT = 0;

            Macro_Clear_Bit(TIM4->SR, 0);
            NVIC_ClearPendingIRQ(TIM4_IRQn);

            Macro_Set_Bit(TIM4->CR1, 0);
        }

        else if(!k && Key_State == 1)
        {
            Key_State = 0;

            Macro_Clear_Bit(TIM4->CR1, 0);

            if(TIM4_Expired || (TIM4->SR & 1))
                Key_Pressed = 2;
            else
                Key_Pressed = 1;
        }

        EXTI->PR = (1 << 7);
        NVIC_ClearPendingIRQ(23);
    }
}

extern volatile int Uart_Data_In;
extern volatile unsigned char Uart_Data;

void USART2_IRQHandler(void)
{
	Uart_Data = (unsigned char)USART2->DR;
	Uart_Data_In = 1;
	NVIC_ClearPendingIRQ(38);
}
