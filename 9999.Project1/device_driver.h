#include "stm32f4xx.h"
#include "option.h"
#include "macro.h"

extern void Uart2_Init(int baud);
extern void Uart2_RX_Interrupt_Enable(int en);

extern void LED_Init(void);
extern void LED_state(int led_s);

extern void Clock_Init(void);

extern void Key_Poll_Init(void);
extern void Key_ISR_Enable(int en);

extern void Motor_Init(void);
extern void Motor_SC(int turn,unsigned int duty);

extern void TIM4_Repeat_Interrupt_Enable(int en, int time);
