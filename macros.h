#ifndef MACROS_H_INCLUDED
#define MACROS_H_INCLUDED

#define TRUE                        1
#define FALSE                       0
#define PI                          3.14159

#define ONE_SECOND                  168000
#define STM32_CLOCK_HZ              168000000UL
#define STM32_CYCLES_PER_LOOP       10

// PWM macros
#define PULSE_ONE_MS                4700
#define PULSE_ONE_PERCENT           47
#define GPIO_PWM                    GPIOB
#define PWM_CLOCK                   RCC_AHB1Periph_GPIOB
#define PWM1_PIN                    GPIO_Pin_6
#define PWM2_PIN                    GPIO_Pin_7
#define PWM3_PIN                    GPIO_Pin_8
#define PWM4_PIN                    GPIO_Pin_9
#define PWM1_PIN_SOURCE             GPIO_PinSource6
#define PWM2_PIN_SOURCE             GPIO_PinSource7
#define PWM3_PIN_SOURCE             GPIO_PinSource8
#define PWM4_PIN_SOURCE             GPIO_PinSource9

#define RIGHT_MOTOR_FUNC            TIM_SetCompare1     //PB6
#define RIGHT_MOTOR_PULSE           4700

#define LEFT_MOTOR_FUNC             TIM_SetCompare2      //PB7
#define LEFT_MOTOR_PULSE            4700

#define BACK_MOTOR_FUNC             TIM_SetCompare3      //PB8
#define BACK_MOTOR_PULSE            4700

#define FRONT_MOTOR_FUNC            TIM_SetCompare4      //PB9
#define FRONT_MOTOR_PULSE           4700

#define P_ROLL                      25

// led macros
#define GPIO_LED                    GPIOD
#define LED_CLOCK                   RCC_AHB1Periph_GPIOD
#define LED1_PIN                    GPIO_Pin_12
#define LED2_PIN                    GPIO_Pin_13
#define LED3_PIN                    GPIO_Pin_14
#define LED4_PIN                    GPIO_Pin_15

//Gyro Address: 1101011
//Acc Address: 0011001
//Mag Address: 0011110

// IMU macros
#define IMU_GYRO_ADDR               0b1101011
#define IMU_ACCEL_ADDR              0b0011001
#define IMU_MAG_ADDR                0b0011110
#define GAIN                        100
#define COMP_GAIN                   .95

#endif /* MACROS_H_INCLUDED */
