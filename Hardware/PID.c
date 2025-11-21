#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "Serial.h"
#include <math.h>

#define MAX_OUT ((float)1000)

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
	float Error1;			// 上次误差
	float Error2;			// 上上次误差
	float Error0;			// 当前误差
	float SumError;			// 误差积分
	uint16_t Count1;
	uint16_t Count2;
} PID_Data_Typedef;

typedef struct
{
	uint8_t Motor_Num;
	PID_Mode Mode;
	PID_Data_Typedef PID_Data_Structure;
} PID_Tick_Typedef;


typedef enum {
	POSTION = 0,
	ADDITION = 1
} PID_Mode;
/**
 * @brief PID调控电机速度函数
 * @param Motor_Num 电机编号，从1开始编号
 * @param pid 存储PID参数等相关数据的结构体地址
 * @param Mode PID模式，位置式或者增量式
 * @retval 无，仅对pid对应的结构体数据进行修改
 */
void PID_Motor_Control(uint8_t Motor_Num, PID_Data_Typedef *pid, PID_Mode Mode)
{
	pid->Error2 = pid->Error1;
	pid->Error1 = pid->Error0;
	pid->Error0 = pid->Current - pid->Target;
	pid->SumError += pid->Error0;

	float kp = pid->Kp;
	float ki = pid->Ki;
	float kd = pid->Kd;

	float Out_P = 0;
	float Out_I = 0;
	float Out_D = 0;

	if (Mode == POSTION)
	{
		/* 位置式PID */
		/* 比例项 */
		Out_P = kp * pid->Error0;
		/* 积分项 */
		Out_I = ki * pid->SumError;
		/* 微分项 */
		Out_D = kd * (pid->Error0 - pid->Error1);

		pid->Out = Out_P + Out_I - Out_D;
	}
	else if (Mode == ADDITION)
	{
		/* 增量式PID */
		/* 比例项 */
		Out_P = kp * (pid->Error0 - pid->Error1);						// 计算输出P
		Out_P *= pid->Magnification;
		/* 积分项 */
		Out_I = ki * (pid->Error0);										// 计算输出I
		/* 微分项 */
		Out_D = kd * (pid->Error0 - 2 * pid->Error1 + pid->Error2);		// 计算输出D
		/* 输出 */
		pid->Out += Out_P + Out_I - Out_D;
	}
	else 
	{
		return;
	}

	pid->P = Out_P;
	pid->I = Out_I;
	pid->D = Out_D;
	/* 输出限幅 */
	if (pid->Out >= MAX_OUT)
	{
		pid->Out = MAX_OUT;
	}
	else if (pid->Out <= -MAX_OUT)
	{
		pid->Out = -MAX_OUT;
	}

}

void PID_TypedefStructInit(PID_Data_Typedef *PID_Struct)
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
	PID_Struct->Error1 = 0;
	PID_Struct->Error2 = 0;	
	PID_Struct->Error0 = 0;
	PID_Struct->SumError = 0;
	PID_Struct->Count1 = 0;
	PID_Struct->Count2 = 0;
}

void PID_TypedefStructReset(PID_Data_Typedef *PID_Struct)
{
	PID_Struct->Current = 0;
	// PID_Struct->Target = 0;
	PID_Struct->P = 0;
	PID_Struct->I = 0;
	PID_Struct->D = 0;
	PID_Struct->Out = 0;
	PID_Struct->Error1 = 0;
	PID_Struct->Error2 = 0;	
	PID_Struct->Error0 = 0;
	PID_Struct->SumError = 0;
	PID_Struct->Count1 = 0;
	PID_Struct->Count2 = 0;
}

void PID_Tick(PID_Tick_Typedef *PID_Tick_Structure)
{
	static uint16_t count = 0;
	if (count >= 10)
	{
		PID_Motor_Control(PID_Tick_Structure->Motor_Num,
			 &(PID_Tick_Structure->PID_Data_Structure),
			  PID_Tick_Structure->Mode);
		count = 0;
	}
	count++;
}
