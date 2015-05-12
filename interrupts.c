#include "interrupts.h"

// Initializes the interrupts. This means that we set up
// the proper timer for the interrupts and enable it.
// @param: none
// @return: none
void int_init()
{
    int_init_timer();
    int_enable_interrupt();
    g_transmit_time = 500; // Default value 500 (500ms)
}

// Sets up the timer that will cause the interrupt. In this function,
// the frequency of interrupts is determined.
// @param: none
// @return: none
void int_init_timer()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    TIM_TimeBaseInitTypeDef TIM_BaseStructure;
    // frequency = ((clockspeed/2) / prescaler) / period
    // (168000000/2) / 16800 / 100 = 80 (About 100ms)
    TIM_BaseStructure.TIM_Prescaler = 60000;
    TIM_BaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseStructure.TIM_Period = 140;
    TIM_BaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_BaseStructure);
    TIM_Cmd(TIM2, ENABLE);
}

// Sets the priority for the interrupt of timer 2. Also starts
// the timer.
// @param: none
// @return: none
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

// Timer2 interrupt handler. This is what happens when the timer2
// goes off.
// @param: none
// @return: none
void TIM2_IRQHandler()
{
	static int counter = 1;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // reset the timer flag --> you HAVE to do this or else it will keep interrupting
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        GPIO_ToggleBits(GPIOD, GPIO_Pin_12);

        // Interrupt is set to transmit every 100ms. g_interrupt_time allows
        // the rate of transmission time to be set during runtime. Counter keeps
        // track of the number of interrupts called. Comparing counter with g_interrupt_time
        // creates a second pseudo-timer. ie. if g_interrupt_time is set to 500 (500ms)
        // then counter should count up to 5 and then transmit the packet
        // because that means 5 interrupts have occurred and therefore 500ms have passed.
        if (counter*100 >= g_transmit_time)
        {
            uint8_t packet[PACKET_SIZE] = {0};
            protocol_packet_generator(packet,g_queue);
            counter = 1;
        }
        else
        {
            counter++;
        }
    }
}
