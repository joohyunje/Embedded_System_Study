#include "device_driver.h"
#include <stdio.h>

#define Stop   0
#define Forward   1
#define Reverse   2

static void Sys_Init(int baud)
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2);
	Clock_Init();
	Uart2_Init(baud);
    setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
	Motor_Init();
	Key_Poll_Init();
}

volatile int Uart_Data_In = 0;
volatile unsigned char Uart_Data = 0;
volatile int TIM4_Expired=0;
volatile int Key_Pressed=0;

void Main(void)
{
	Sys_Init(115200);
    Uart2_RX_Interrupt_Enable(1);
	Key_ISR_Enable(1);
	TIM4_Repeat_Interrupt_Enable(1, 2000);
	int duty =100;
	int status =Stop;
	int Start_flag=0;

    for(;;)
    {
		if(Key_Pressed)
		{
			if(Key_Pressed == 1 && status !=Stop){
				if(status == Forward){
					status = Reverse;
				}
				else if(status == Reverse){
					status = Forward;
				}
				printf("[Button] Short press | Direction: %s | PWM duty: %d%%\n",
					status == Forward ? "Forward" : "Reverse", duty);
			}
			else if(Key_Pressed ==2){
				if(status == Stop && Start_flag ==0){
					Start_flag =1;
					Uart_Data = 0;
				}
				else {
					status = Stop;
					Start_flag =0;
				}
				printf("[Button] Long press | %s\n", Start_flag
					? "Motor control enabled. Press F or R to start."
					: "Motor stopped. Motor control disabled.");
			}
			Motor_SC(status,duty);
		}
		if(Uart_Data_In && Start_flag ==1)
        {
            if ((Uart_Data >= '1') && (Uart_Data <= '9') && (status !=Stop))
            {
                int value = Uart_Data - '0';
                duty = 50 + (value - 1) * 50 / 8;
                printf("[Motor] PWM duty set to %d%%\n", duty);
            }
            else{
				switch(Uart_Data)
				{
					case 's':
					case 'S':
						status = Stop;
						printf("[Motor] Stopped\n");
						break;
					case 'f':
					case 'F':
						status = Forward;
						printf("[Motor] Forward | PWM duty: %d%%\n", duty);
						break;
					case 'r':
					case 'R':
						printf("[Motor] Reverse | PWM duty: %d%%\n", duty);
						status = Reverse;
						break;
					default:
						break;
				}
			}
			Uart_Data_In = 0;
			Motor_SC(status,duty);
        }
		Key_Pressed = 0;
		LED_state(status);
    }
}
