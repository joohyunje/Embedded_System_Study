#include "device_driver.h"


void Motor_Init(void){
    Macro_Set_Bit(RCC->AHB1ENR,0);

    Macro_Write_Block(GPIOA->MODER, 0xf, 0x5, 0);
    Macro_Clear_Area(GPIOA->OTYPER,0x3, 0);
}

void Motor_R(void){
    LED_Off();
	Macro_Set_Bit(GPIOA->ODR,0);
	Macro_Clear_Bit(GPIOA->ODR,1);
}

void Motor_F(void){
    LED_On();
    Macro_Set_Bit(GPIOA->ODR,1);
	Macro_Clear_Bit(GPIOA->ODR,0); 
   
}

void Motor_S(void){
    LED_Off();
    Macro_Clear_Bit(GPIOA->ODR,0);
    Macro_Clear_Bit(GPIOA->ODR,1);
    TIM2_Delay(500);
}

void Motor_Invert(void){
    if(Macro_Check_Bit_Clear(GPIOA->ODR,0)){
        Motor_S();
        Motor_R();
    }
    else {
        Motor_S();
        Motor_F();
    }
    
}