#include "macros.h"
#include <stdio.h>
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "core_cm4.h"
#include "misc.h"
#include "Filter.h"
#include <math.h>
#include "pwm.h"

extern int count;
typedef struct{int x, y, z; float xang, yang, zang;}ACC;
typedef struct{float x, y, z; float xang, yang, zang; }GYRO;
typedef struct{float x, y, z, norm;}MAG;
typedef struct{int comp_x, comp_y, comp_z;int kal_x, kal_y;float yaw;}ANGLE;
MAG mag;
ACC acc;
ANGLE ang;
GYRO gyro;

void timer_init()
{
	//configure timer2 for internal timer interrupt
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef timerInitStructure;
	timerInitStructure.TIM_Prescaler = 100-1;//105
	timerInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	timerInitStructure.TIM_Period = 8400-1;//2200
	timerInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	timerInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &timerInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

}

void EnableTimerInterrupt()
{
	//enable timer interrupt
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
}

void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
    	float dt= .085;
    	float Xh,Yh;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        //Read gyro x, y, z axes
        gyro.x = read_gyro(0x29,0x28);
        gyro.y = read_gyro(0x2B,0x2A);
        gyro.z = read_gyro(0x2D,0x2C);

        //Read accelerometer x, y, z axes
        acc.x = GAIN*read_accelerometer(0x29,0x28);
        acc.y = GAIN*read_accelerometer(0x2B,0x2A);
        acc.z = GAIN*read_accelerometer(0x2D,0x2C);

        //Read magnetometer x, y, z axes
        mag.x = read_mag(0x03,0x04);
        mag.z = read_mag(0x05,0x06);
        mag.y = read_mag(0x07,0x08);
        //mag.norm = sqrtf((mag.x*mag.x)+(mag.y*mag.y)+(mag.z*mag.z));
        //mag.x = mag.x/mag.norm;
        //mag.y = mag.y/mag.norm;
        //mag.z = mag.z/mag.norm;

        //Calculate Angles with respect to axes using accelerometer
        acc.yang = (180/PI*atan2f((float)acc.x,(sqrtf(acc.y*acc.y+acc.z*acc.z))));
        acc.xang = (180/PI*atan2f((float)acc.y,(sqrtf(acc.x*acc.x+acc.z*acc.z))));
        acc.zang = (180/PI*atan2f((sqrtf(acc.x*acc.x+acc.y*acc.y)),(float)acc.z));

        //Calculate angles with respect to axes using gyroscope
        gyro.xang += gyro.x*dt;
        gyro.yang += gyro.y*dt;
        gyro.zang += gyro.z*dt;

        //Complementary Filter
        ang.comp_x = (COMP_GAIN*(float)(ang.comp_x + gyro.x*dt) + ((1-COMP_GAIN)*(float)acc.xang));
        ang.comp_y = (COMP_GAIN*(float)(ang.comp_y + gyro.y*dt) + ((1-COMP_GAIN)*(float)acc.yang));

        //Kalman Filter
//        ang.kal_x = kalman(acc.xang,gyro.x,dt);
//        ang.kal_y = kalman(acc.yang,gyro.y,dt);

        //Print roll angles as calculated and measured by all methods
        //printf("%d\t%d\t%d\t%d\n",(int)gyro.xang,(int)acc.xang,(int)ang.comp_x,(int)ang.kal_x);

        //Calculate Yaw angle
        Xh = mag.x*cosf((float)ang.comp_y)+mag.y*sinf((float)ang.comp_x)*sinf((float)ang.comp_y)+mag.z*cosf((float)ang.comp_x)*sinf((float)ang.comp_y);
        Yh = -mag.y*cosf((float)ang.comp_x)+mag.z*sinf((float)ang.comp_x);
        ang.yaw = (180/PI)*atan2f(Yh,Xh);
        ang.comp_z = COMP_GAIN*(float)(ang.comp_z - gyro.z*dt) + ((1-COMP_GAIN)*(float)ang.yaw);

        //Print Yaw
        //printf("%d\t%d\t%d\n",(int)ang.comp_z, (int)ang.yaw, -(int)gyro.zang);
    }
}
