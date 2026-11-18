#include "device_driver.h"
#include <stdio.h>

void _Invalid_ISR(void)
{
	unsigned int r = Macro_Extract_Area(SCB->ICSR, 0x1ff, 0);
	printf("\nInvalid_Exception: %d!\n", r);
	printf("Invalid_ISR: %d!\n", r - 16);
	for(;;);
}

volatile int TIM4_Expired = 0;

void EXTI9_5_IRQHandler(void)
{
    if(EXTI->PR & (1 << 7))
    {
        // 버튼 누름 
        if(Macro_Check_Bit_Clear(GPIOC->IDR, 7))
        {
            TIM4_Expired = 0;

            Macro_Clear_Bit(TIM4->CR1, 0);
            TIM4->CNT = 0;

            // 타이머 인터럽트초기화
            Macro_Clear_Bit(TIM4->SR, 0);
            NVIC_ClearPendingIRQ(TIM4_IRQn);

            // TIM4 시작
            Macro_Set_Bit(TIM4->CR1, 0);
        }
        else
        {
            // TIM4 정지
            Macro_Clear_Bit(TIM4->CR1, 0);

            if(TIM4_Expired || (TIM4->SR & 1))
            {
                Key_Pressed = 2;  // Long
            }
            else
            {
                Key_Pressed = 1;  // Short
            }
        }

        // EXTI7 Pending Clear
        EXTI->PR = (1 << 7);
		NVIC_ClearPendingIRQ(23);
    }
}

void TIM4_IRQHandler(void)
{
	Macro_Clear_Bit(TIM4->SR, 0);
	NVIC_ClearPendingIRQ(30);
	TIM4_Expired = 1;
}

volatile int DMA2_STREAM0_DONE = 0;

void DMA2_Stream0_IRQHandler(void)
{
	DMA2->LIFCR = 0x3F << 0;
	NVIC_ClearPendingIRQ(56);
    DMA2_STREAM0_DONE = 1;
}

volatile int DMA1_STREAM6_DONE = 0;

void DMA1_Stream6_IRQHandler(void)
{
	DMA1->HIFCR = 0x3F << 16;
	NVIC_ClearPendingIRQ(17);
    DMA1_STREAM6_DONE = 1;
}
