#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Servo_Init(void)
{
	PWM_Init();
}

void Servo_SetAngle(float Angle)
{
	// CRR  500 <=> 0   degree
	// CRR 2500 <=> 180 degree
	PWM_SetCompare2(Angle / 180 * 2000 + 500);
}
