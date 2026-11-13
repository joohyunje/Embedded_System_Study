#include "device_driver.h"
#include <stdio.h>

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
    setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
}

void Main(void)
{
	Sys_Init(115200);

	int a;
	float b;
	char s[20];

	printf("\nCompiler Test\n");

	printf("\n정수 문자열 실수를 하나씩 공백으로 구분하여 입력, 뒤로가기는 지원하지 않음\n");
	printf("(예)100 hello 3.14\n");
	
	scanf("%d %s %f", &a, s, &b);
	printf("RCV = %d, %s, %f\n", a, s, b);

	printf("입력한 내용이 잘 인쇄되면 정상임\n");
}
