#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "Serial.h"
#include <math.h>

typedef struct {
	float Magnification;  	// 倍数，表示最终结果要乘多少倍
	float Kp;				// 比例项系数
	float Ki;				// 积分项系数
	float Kd;				// 微分项系数
	float Current;			// 当前速度
	float Target;			// 目标速度
	float P;				// 比例项结果
	float I;				// 积分项结果
	float D;				// 微分项结果
	float Out;				// 输出
	float PrevError;		// 上次误差
	float PrevPrevError;	// 上上次误差
	float CurrError;		// 当前误差
	float SumError;			// 误差积分
	uint16_t Count1;
	uint16_t Count2;
} PID_Typedef;

typedef enum {
	POSTION = 0,
	ADDITION = 1
} PID_Mode;

void PID_Motor_Control(uint8_t Motor_Num, PID_Typedef *pid, PID_Mode Mode)
{
	
}

void PID_TypedefStructInit(PID_Typedef *PID_Struct)
{
	PID_Struct->Magnification = 1;
	PID_Struct->Kp = 1;
	PID_Struct->Ki = 0.01;
	PID_Struct->Kd = 1;
	PID_Struct->Current = 0;
	PID_Struct->Target = 0;
	PID_Struct->P = 0;
	PID_Struct->I = 0;
	PID_Struct->D = 0;
	PID_Struct->Out = 0;
	PID_Struct->PrevError = 0;
	PID_Struct->PrevPrevError = 0;	
	PID_Struct->CurrError = 0;
	PID_Struct->SumError = 0;
	PID_Struct->Count1 = 0;
	PID_Struct->Count2 = 0;
}

void PID_TypedefStructReset(PID_Typedef *PID_Struct)
{
	PID_Struct->Current = 0;
	// PID_Struct->Target = 0;
	PID_Struct->P = 0;
	PID_Struct->I = 0;
	PID_Struct->D = 0;
	PID_Struct->Out = 0;
	PID_Struct->PrevError = 0;
	PID_Struct->PrevPrevError = 0;	
	PID_Struct->CurrError = 0;
	PID_Struct->SumError = 0;
	PID_Struct->Count1 = 0;
	PID_Struct->Count2 = 0;
}
