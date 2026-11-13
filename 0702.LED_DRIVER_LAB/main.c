#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud) 
{
    SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Uart2_Init(baud);	
	setvbuf(stdout, NULL, _IONBF, 0);
}

void Main(void)
{
	Sys_Init(115200);
	printf("LED Driver\n");

	volatile int i, j;
	int led = 0;

	LED_Init();

	for(i = 0; i < 10; i++)
	{
		LED_Display(led ^= 1);
		for(j=0; j<0x40000; j++);
	}

	for(;;)
	{
		LED_Off();
		for(j=0; j<0x10000; j++);
		LED_On();
		for(j=0; j<0x10000; j++);
	}
}