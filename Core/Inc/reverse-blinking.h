/**
 * @file    reverse-blinking.h
 * @brief   Makes LD3-LD4 leds go around in a direction, which can be reversed by pressing 
 *          the B1 button.
 */

#ifndef __REVERSE_BLINKING_H
#define __REVERSE_BLINKING_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stm32f4xx_hal.h"

typedef uint16_t u16;
typedef int32_t  i32;
typedef int32_t  bool;

#define N_LEDS 4
#define STEP_TIME_MS 500

u16 led_pins[N_LEDS] = {LD3_Pin, LD5_Pin, LD6_Pin, LD4_Pin};
u16 led_idx = 0;
bool is_clockwise = 1;

void step_led_idx(void) {
    if (is_clockwise) {
        led_idx = (led_idx + 1) % N_LEDS; 
    } else {
        led_idx = ((led_idx - 1) + N_LEDS) % N_LEDS;
    }
}

void HAL_SYSTICK_Callback(void) {
    if (HAL_GetTick() % STEP_TIME_MS == 0) {
        HAL_GPIO_WritePin(GPIOD, led_pins[led_idx], 0);
        step_led_idx();
        HAL_GPIO_WritePin(GPIOD, led_pins[led_idx], 1);
    }
}

void HAL_GPIO_EXTI_Callback(u16 port) {
    is_clockwise = !is_clockwise;
}

#ifdef __cplusplus
}
#endif

#endif /* __REVERSE_BLINKING_H */
