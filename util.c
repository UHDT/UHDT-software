#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "util.h"
#include "macros.h"

static __IO uint32_t util_timing_delay = 0;
static __IO uint32_t util_time = 0;
uint8_t util_delay_initialized = 0;

#ifdef KEIL_IDE
void TimingDelay_Decrement(void) {
	util_time++;
	if (util_time2 != 0x00) {
		util_time2--;
	}
	if (util_timing_delay != 0x00) {
		util_timing_delay--;
	}
}
#else
void SysTick_Handler(void) {
	util_time++;
	if (util_time2 != 0x00) {
		util_time2--;
	}
	if (util_timing_delay != 0x00) {
		util_timing_delay--;
	}
}
#endif


// Initialize the GPIO pins for the LEDs.
// @param: none
// @return: none
void util_leds_init()
{
    GPIO_InitTypeDef GPIO_InitStruct;

    // initiliaze clock for leds
    RCC_AHB1PeriphClockCmd(LED_CLOCK, ENABLE);

    // set pins for leds
    GPIO_InitStruct.GPIO_Pin = LED1_PIN | LED2_PIN | LED3_PIN | LED4_PIN;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIO_LED, &GPIO_InitStruct);
}
// Cap a value within a certain range.
// @param: value - the value that you want to cap
// @param: minimum - the minimum value that you can go to
// @param: maximum - the maximum value that you want to go to
// @return: none
void util_cap_value(int *value, int minimum, int maximum)
{
    if (*value > maximum) {
        *value = maximum;
    } else if (*value < minimum) {
        *value = minimum;
    }
}

// Delay for a certain amount of nanoseconds. This only works
// if you set the clock to 180MHz.
// @param: nTime - the amount of nanoseconds you want to delay
// @return: none
void util_delay_ns(__IO uint32_t nTime) {
	util_timing_delay = nTime;

	while (util_timing_delay != 0);
}

// Delay for a certain amount of milliseconds. This only works
// if you set the clock to 180MHz.
// @param: mTime - number of milliseconds you want to delay
// @return: none
void util_delay_ms(__IO uint32_t mTime) {
	util_timing_delay = 1000 * mTime;

	while (util_timing_delay != 0);
}

// Initialize the delay functions.
// @param: none
// @return: none
void util_delay_init(void) {
	if (util_delay_initialized) {
		return;
	}
	RCC_HSEConfig(RCC_HSE_ON);
	while (!RCC_WaitForHSEStartUp());
	/* Set Systick interrupt every 1us */
	if (SysTick_Config(SystemCoreClock / 1000000)) {
		/* Capture error */
		while (1);
	}
	util_delay_initialized = 1;
}

// Gives you the current time (time 1). Used to do stuff every
// x amount of time.
// @param: none
// @return: the current time (time 1)
uint32_t util_delay_time(void) {
	return util_time;
}

// Gives you the current time (time 2). Used to do stuff every
// x amount of time.
// @param: none
// @return: the current time (time 2)
uint32_t util_delay_time2(void) {
	return util_time2;
}

// Set the current time (time 1)
// @param: time you want to set current time to (time 1)
// @return: none;
void util_delay_set_time(uint32_t time) {
	util_time = time;
}

// Set the current time (time 2)
// @param: time you want to set current time to (time 2)
// @return: none;
void util_delay_set_time2(uint32_t time) {
	util_time2 = time;
}

// Re-enable Systick. It has to be configured before with util_delay_init();
// @param: none
// @return: none
void util_delay_enable_systick(void) {
	if (!util_delay_initialized) {
		return;
	}

	/* Enable systick interrupts, useful when you wakeup from sleep mode */
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

// Disable Systick. Usef when going into sleep mode, so systick will not
// @param: none
// @return: none
void util_delay_disable_systick(void) {
	/* Disable systick, useful when you go to sleep mode */
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}




