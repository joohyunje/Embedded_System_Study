#include "stm32f4xx.h"
#include "option.h"
#include "macro.h"
#include "malloc.h"

typedef enum{
	EVENT_NONE,
	EVENT_KEY_Short,
	EVENT_KEY_Long,
	EVENT_STOP,
	EVENT_FORWARD,
	EVENT_REVERSE
}EVENT;

// Uart.c

extern void Uart2_Init(int baud);
extern void Uart1_Init(int baud);

// Led.c

extern void LED_Init(void);
extern void LED_On(void);
extern void LED_Off(void);
extern void LED_Display(int on);

// Clock.c

extern void Clock_Init(void);

// Key.c

extern void Key_Poll_Init(void);
extern int Key_Get_Pressed(void);
extern void Key_Wait_Key_Released(void);
extern void Key_Wait_Key_Pressed(void);
extern void Key_ISR_Enable(int en);

// Motor.c

extern void Motor_Init(void);
extern void Motor_SC(int turn,unsigned short duty);
extern void Motor_Stop(void);



// Timer.c

extern void TIM2_Delay(int time);
extern void TIM2_Stopwatch_Start(void);
extern unsigned int TIM2_Stopwatch_Stop(void);
extern void TIM4_Repeat(int time);
extern int TIM4_Check_Timeout(void);
extern void TIM4_Stop(void);
extern void TIM4_Change_Value(int time);
extern void TIM3_Out_Init(void);
extern void TIM3_Out_Freq_Generation(unsigned short freq);
extern void TIM3_Out_Stop(void);
