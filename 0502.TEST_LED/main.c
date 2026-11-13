// 여기에 사용자 임의의 define을 작성하시오

#define GPIOA_MODER		((unsigned long *)0x40020000) // 4바이트 지정하기 위해 int, long은 고정으로 4바이트라 컴파일러에 영향을 안 받으려면 long으로 하는게 좋음
#define GPIOA_OTYPER	(0x40020004)
#define GPIOA_ODR		(*(unsigned long *)0x40020014)

void Main(void)
{
	// LED GPA[5]를 출력(General Push Pull) 모드로 설정하시오

	*GPIOA_MODER = 0x00000400;
	*(unsigned long*)GPIOA_OTYPER = 0x00000000;

	// GPA[5] LED를 ON 시키도록 설정하시오

	GPIOA_ODR = 0x00000020;
}
