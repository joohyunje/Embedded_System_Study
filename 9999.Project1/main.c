#include "device_driver.h"
#include <stdio.h>

static EVENT Event = EVENT_NONE;
static EVENT Status = EVENT_STOP;

static void Sys_Init(int baud) 
{
	SCB->CPACR |= (0x3 << 10*2)|(0x3 << 11*2); 
	Clock_Init();
	Uart2_Init(baud);
    setvbuf(stdout, NULL, _IONBF, 0);
	LED_Init();
	Motor_Init();
}

static void Key_Event_Handler(void)
{
    static int flag = 0;
    int key = Key_Get_Pressed();

    // UART에 수신된 문자가 있을 때만 읽는다고 가정

    if(flag == 0 && key)
    {
        flag = 1;
        Event =  EVENT_KEY;
    }
    else if(flag == 1 && !key)
    {
        flag = 0;
    }

}

static void Uart_Event_Handler(void)
{
    if (Macro_Check_Bit_Set(USART2->SR,5))
    {
        char ch = USART2->DR;;

        switch(ch)
        {
            case 's':
            case 'S':
                Event = EVENT_STOP;
				break;

            case 'f':
            case 'F':
                Event = EVENT_FORWARD;
				break;
            case 'r':
            case 'R':
                Event = EVENT_REVERSE;
				break;
			default:
				break;	
        }
    }
}

static void Op_Handler(EVENT event){
	switch (event)
	{
	case EVENT_KEY:
		if(Status !=EVENT_STOP){
			Motor_Invert();
		}
		printf("keykey\n");
		break;

	case EVENT_STOP:
		Motor_S();
		Status = EVENT_STOP;
	    printf("Motor Stop\n");
		
		break;
		
	case EVENT_FORWARD:
		
		if((Status == EVENT_REVERSE) || (Status == EVENT_KEY))
		{
			Motor_Invert();
		}	
		else
		{
			Motor_F();
		} 
		
		Status = EVENT_FORWARD;
	    printf("Motor Forward\n");
		
		break;
		
	case EVENT_REVERSE:
		
		if((Status == EVENT_FORWARD) || (Status == EVENT_KEY))
		{
			Motor_Invert();
		}	
		else
		{
			Motor_R();
		} 
		
		Status = EVENT_REVERSE;
    	printf("Motor Reverse\n");
		
		break;	
	
		
	default:
		break;
	}
	
	Event = EVENT_NONE;
	
}



void Main(void)
{
	Sys_Init(115200);


	for(;;){

		Key_Event_Handler();
		Uart_Event_Handler();
		Op_Handler(Event);

	}
}
