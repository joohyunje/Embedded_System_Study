#include "device_driver.h"

#define TIM_FREQ                   (8000000.)                   // Hz

void Motor_Init(void)
{
    Macro_Set_Bit(RCC->AHB1ENR, 0); // GPIOA On
    Macro_Set_Bit(RCC->APB1ENR, 0); // TIM2 On
    Macro_Set_Bit(RCC->APB1ENR, 3); // TIM5 On

    Macro_Write_Block(GPIOA->MODER, 0xf, 0xA, 0);      // PA0, 1 => ALT
    Macro_Write_Block(GPIOA->AFR[0], 0xff, 0x21, 0);     // PA0 => AF01, PA1 => AF02

    Macro_Write_Block(TIM2->CCMR1, 0xff, 0x68, 0);
    Macro_Write_Block(TIM5->CCMR1, 0xff, 0x68, 8);
    TIM2->CCER = (1 << 0); // 
    TIM5->CCER = (1 << 4); // 
    TIM2->CCR2 = 0;
    TIM5->CCR2 = 0;

}

void Motor_SC(int turn, unsigned short duty)
{
    if (duty > 100) {
        duty = 100;
    }
    else if(duty < 50){
        duty = 50;
    }

    if(turn == 1){
        TIM2->PSC = (int)(TIMXCLK/TIM_FREQ + 0.5) -1;
        TIM2->ARR = (int)(TIM_FREQ)/0xffff;
        TIM2->CCR1 = (TIM2->ARR * (unsigned int)duty) / 100U;
        Macro_Set_Bit(TIM2 -> EGR , 0);
        TIM2->CR1 = (1<<4) | (0<<3) | (1<<0);
        TIM5->CCR2 = 0;
    }
    else if(turn == 2){
        TIM5->PSC = (int)(TIMXCLK/TIM_FREQ + 0.5) -1;
        TIM5->ARR = (int)(TIM_FREQ)/0xffff;
        TIM5->CCR2 = (TIM5->ARR * (unsigned int)duty) / 100U;
        Macro_Set_Bit(TIM5 -> EGR , 0);
        TIM5->CR1 = (1<<4) | (0<<3) | (1<<0);
        TIM2->CCR2 = 0;
    }
}

void Motor_Stop(void)
{
    TIM2->CCR2 = 0;
    TIM5->CCR2 = 0;
}