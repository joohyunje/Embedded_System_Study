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

#if 0

void Main(void)
{
	Sys_Init(115200);
	printf("Frequency Generation Test!!\n");

	TIM3_Out_Init();
	TIM3_Out_Freq_Generation(1000);
	TIM2_Delay(1000);
	TIM3_Out_Stop();
}

#endif

#if 1	 

#define BASE  (500) //msec

static void Buzzer_Beep(unsigned char tone, int duration)
{
	const static unsigned short tone_value[] = {261,277,293,311,329,349,369,391,415,440,466,493,523,554,587,622,659,698,739,783,830,880,932,987,1046,1109,1175,1245,1319,1397, 1480,1568,1661,1760,1865,1976};

	TIM3_Out_Freq_Generation(tone_value[tone]);
	TIM2_Delay(duration);
	TIM3_Out_Stop();
}

void Main(void)
{
	Sys_Init(115200);
	printf("Buzzer Test!!\n");

	int i;
	enum key{C1, C1_, D1, D1_, E1, F1, F1_, G1, G1_, A1, A1_, B1, C2, C2_, D2, D2_, E2, F2, F2_, G2, G2_, A2, A2_, B2, C3, C3_, D3, D3_, E3, F3, F3_, G3, G3_, A3, A3_, B3};
	enum note{N16=BASE/4, N8=BASE/2, N4=BASE, N2=BASE*2, N1=BASE*4};
	const int song1[][2] = {{G1,N4},{G1,N4},{E1,N8},{F1,N8},{G1,N4},{A1,N4},{A1,N4},{G1,N2},{G1,N4},{C2,N4},{E2,N4},{D2,N8},{C2,N8},{D2,N2}};
	const int boot_sound[][2] = {
    {D3_, N4},
	{D2_, N16},
    {A2_, N8},
    {G2_, N8},
    {D2_, N4},
    {D3_, N8},
    {A2_, N2}
};
	const int shutdown_song[][2] = {{C2, N8}, {G1, N8}, {E1, N8}, {C1, N4}};
	const char * note_name[] = {"C1", "C1#", "D1", "D1#", "E1", "F1", "F1#", "G1", "G1#", "A1", "A1#", "B1", "C2", "C2#", "D2", "D2#", "E2", "F2", "F2#", "G2", "G2#", "A2", "A2#", "B2", "C3", "C3#", "D3", "D3#", "E3", "F3", "F3#", "G3", "G3#", "A3", "A3#", "B3"};

	TIM3_Out_Init();

	printf("\nSong Play\n");

	for(i=0; i<(sizeof(shutdown_song)/sizeof(shutdown_song[0])); i++)
	{
		printf("%s ", note_name[shutdown_song[i][0]]);
		Buzzer_Beep(shutdown_song[i][0], shutdown_song[i][1]);
	}
}

#endif