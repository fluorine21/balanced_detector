/*
 * hal_wrapper.h
 *
 *  Created on: May 31, 2021
 *      Author: James Williams
 */

#ifndef APPLICATION_USER_CORE_C_DRIVERS_HAL_WRAPPER_H_
#define APPLICATION_USER_CORE_C_DRIVERS_HAL_WRAPPER_H_

#include "stm32l0xx_hal.h"
//Typedefs to make my life easier
#define u8 uint8_t
#define u16 uint16_t



void set_left_attenuation(u8 val);
void set_right_attenuation(u8 val);
void set_diff_gain(u8 val);
void set_amp_enable(u8 val);
u8 read_adc(u8 sel, u16 *val);
u8 get_uart_byte(u8 * res);
u8 send_uart_byte(u8 * bt);
void set_led(u8 num, u8 val);

#endif /* APPLICATION_USER_CORE_C_DRIVERS_HAL_WRAPPER_H_ */
