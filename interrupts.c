#include <stm32f4xx_tim.h>
#include <stm32f4xx_rcc.h>
#include <misc.h>
#include "util.h"
#include "interrupts.h"

void int_init()
{
    int_init_timer();
    int_enable_interrupt();
}

void int_init_timer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_BaseStructure;
    TIM_BaseStructure.TIM_Prescaler = 40000;
    TIM_BaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStructure.TIM_Period = 500;
    TIM_BaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_BaseStructure);
    TIM_Cmd(TIM2, ENABLE);
}

void int_enable_interrupt()
{
    NVIC_InitTypeDef nvicStructure;
    nvicStructure.NVIC_IRQChannel = TIM2_IRQn;
    nvicStructure.NVIC_IRQChannelPreemptionPriority = 0;
    nvicStructure.NVIC_IRQChannelSubPriority = 1;
    nvicStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvicStructure);
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
}

void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        GPIO_ToggleBits(UTIL_GPIO_LED, UTIL_LED1_PIN);
    }
}
