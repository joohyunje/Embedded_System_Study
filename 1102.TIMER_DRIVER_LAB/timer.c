#include "device_driver.h"

#define TIM2_ARR_MAX 0XFFFFFFFF
#define TIM2_TICK    20         				/*usec*/
#define TIM2_FREQ    (1000000. / TIM2_TICK)		/*Hz*/
#define TIM2_PLS_1ms (1000./TIM2_TICK)



void TIM2_Stopwatch_Start(void)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);
	
	// TIM2 CR1 설정: down count, one pulse
	TIM2->CR1 = (0x1<<4)|(0x1<<3);
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	TIM2->PSC = (int)(TIMXCLK/TIM2_FREQ+0.5)-1;
	// ARR 초기값 설정 => 최대값 0xFFFFFFFF 설정
	TIM2->ARR = TIM2_ARR_MAX;
	// UG 이벤트 발생
	Macro_Set_Bit(TIM2->EGR,0);
	// TIM2 start
	Macro_Set_Bit(TIM2->CR1,0);
	
}

unsigned int TIM2_Stopwatch_Stop(void)
{
	unsigned int time;	
	
	// TIM2 stop
	Macro_Clear_Bit(TIM2->CR1,0);
	// CNT 초기 설정값 (0xFFFFFFFF)와 현재 CNT의 펄스수 차이를 구하고
	// 그 펄스수 하나가 20usec이므로 20을 곱한값을 time에 저장
	time = ((TIM2->ARR) - (TIM2->CNT)) * 20;
	// 계산된 time 값을 리턴(단위는 usec)
	return time;
	
	
}

void TIM2_Delay(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 0);
	
	// TIM2 CR1 설정: down count, one pulse
	// PSC 초기값 설정 => 20usec tick이 되도록 설계 (50KHz)
	// ARR 초기값 설정 => 요청한 time msec에 해당하는 초기값 설정
	// UG 이벤트 발생
	TIM2->CR1 = (0x1<<4)|(0x1<<3);
	TIM2->PSC = (int)(TIMXCLK/TIM2_FREQ+0.5)-1;
	TIM2->ARR = time*(int)(TIM2_PLS_1ms);
	Macro_Set_Bit(TIM2->EGR,0);
	
	// UIF(Update Interrupt Pending) Clear
	Macro_Clear_Bit(TIM2->SR,0);
	// TIM2 start
	Macro_Set_Bit(TIM2->CR1,0);
	// Wait timeout
	while(!Macro_Check_Bit_Set(TIM2->SR,0));
	// TIM2 Stop
	Macro_Clear_Bit(TIM2->CR1, 0);
}

#define TIM4_ARR_MAX 0XFFFFFFFF
#define TIM4_TICK    20         				/*usec*/
#define TIM4_FREQ    (1000000. / TIM4_TICK)		/*Hz*/
#define TIM4_PLS_1ms (1000./TIM4_TICK)

void TIM4_Repeat(int time)
{
	Macro_Set_Bit(RCC->APB1ENR, 2);
	
	// TIM4 CR1: ARPE=0, down counter, repeat mode
	// PSC(50KHz),  ARR(reload시 값) 설정
	// UG 이벤트 발생
	// Update Interrupt Pending Clear
	// TIM4 start
	TIM4->CR1 = (0x1<<4)|(0x0<<3);
	TIM4->PSC = (int)(TIMXCLK/TIM4_FREQ+0.5)-1;
	TIM4->ARR = time*(int)(TIM4_PLS_1ms);
	Macro_Set_Bit(TIM4->EGR,0);
	
	// UIF(Update Interrupt Pending) Clear
	Macro_Clear_Bit(TIM4->SR,0);
	// TIM2 start
	Macro_Set_Bit(TIM4->CR1,0);

}

int TIM4_Check_Timeout(void)
{
	// 타이머가 timeout 이면 1 리턴, 아니면 0 리턴
	if(Macro_Check_Bit_Set(TIM4->SR,0)){
		Macro_Clear_Bit(TIM4->SR,0);
		return 1;
	}
	return 0;
}

void TIM4_Stop(void)
{
	Macro_Clear_Bit(TIM4->CR1, 0);
}

void TIM4_Change_Value(int time)
{
	TIM4->ARR = 50 * time;
}
