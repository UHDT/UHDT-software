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

void util_delay_ns(__IO uint32_t nTime) {
	util_timing_delay = nTime;

	while (util_timing_delay != 0);
}

void util_delay_ms(__IO uint32_t nTime) {
	util_timing_delay = 1000 * nTime;

	while (util_timing_delay != 0);
}

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

/**
 * Get the util_time variable value
 */
uint32_t util_delay_time(void) {
	return util_time;
}

/**
 * Get the util_time variable value
 */
uint32_t util_delay_time2(void) {
	return util_time2;
}
/**
 * Set value for util_time variable
 */
void util_delay_set_time(uint32_t time) {
	util_time = time;
}
/**
 * Set value for util_time variable
 */
void util_delay_set_time2(uint32_t time) {
	util_time2 = time;
}

/**
 * Re-enable Systick. It has to be configured before with util_delay_init();
 */
void util_delay_enable_systick(void) {
	if (!util_delay_initialized) {
		return;
	}

	/* Enable systick interrupts, useful when you wakeup from sleep mode */
	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

/**
 * Disable Systick. Usef when going into sleep mode, so systick will not
 */
void util_delay_disable_systick(void) {
	/* Disable systick, useful when you go to sleep mode */
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}




