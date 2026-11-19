#include "device_driver.h"

#define TIM_FREQ 	  			(8000000.)

void Motor_Init(void)
{
    Macro_Set_Bit(RCC->AHB1ENR, 0);
    Macro_Set_Bit(RCC->APB1ENR, 3);

	Macro_Write_Block(GPIOA->MODER, 0xf, 0xA, 0);
	Macro_Write_Block(GPIOA->AFR[0], 0xff, 0x22, 0);

	Macro_Write_Block(TIM5->CCMR1, 0xffff, 0x6868, 0);
	TIM5->CCER = (1 << 0) | (1 << 4);
	TIM5->CCR1 = 0;
	TIM5->CCR2 = 0;

    TIM5->PSC = (int)(TIMXCLK/TIM_FREQ + 0.5) -1;
    TIM5->ARR = (int)(TIM_FREQ)/0xffff;
    Macro_Set_Bit(TIM5 -> EGR , 0);
    TIM5->CR1 = (0<<4) | (0<<3) | (1<<0);

}

void Motor_SC(int turn, unsigned int duty)
{
    TIM5->CCR1 = 0;
    TIM5->CCR2 = 0;
    for(volatile int i=0;i<0x8000;i++);
    if (duty > 100) {
        duty = 100;
    }
    else if(duty < 50){
        duty = 50;
    }

    if(turn == 1){

	    TIM5->CCR1 = (TIM5->ARR * duty) / 100;
        TIM5->CCR2 = 0;

    }
    else if(turn == 2){
	    TIM5->CCR2 = (TIM5->ARR * duty) / 100;
        TIM5->CCR1 = 0;

    }
    else if(turn == 0){
        TIM5->CCR1 = 0;
        TIM5->CCR2 = 0;
    }
}
