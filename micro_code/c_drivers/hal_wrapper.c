/*
 * hal_wrapper.c
 *
 *  Created on: May 31, 2021
 *      Author: James A. Williams
 */

//Definitions for the pins
#include "main.h"
#include "hal_wrapper.h"


//Declare the adc instance and uart
extern ADC_HandleTypeDef hadc;
extern UART_HandleTypeDef huart2;


void gpio_init()
{
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
	set_amp_enable(0);
	set_switch(0);
	set_left_attenuation(0);
	set_right_attenuation(0);
	set_diff_gain(0);
}

void set_left_attenuation(u8 val)
{
	//Set each bit one at a time by selecting the relevant bit in val
	HAL_GPIO_WritePin(V5L_GPIO_Port, V5L_Pin, val & (1<<4) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(V4L_GPIO_Port, V4L_Pin, val & (1<<3) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(V3L_GPIO_Port, V3L_Pin, val & (1<<2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(V2L_GPIO_Port, V2L_Pin, val & (1<<1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(V1L_GPIO_Port, V1L_Pin, val & (1<<0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void set_right_attenuation(u8 val)
{
	HAL_GPIO_WritePin(V5R_GPIO_Port, V5R_Pin, val & (1<<4) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(V4R_GPIO_Port, V4R_Pin, val & (1<<3) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(V3R_GPIO_Port, V3R_Pin, val & (1<<2) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(V2R_GPIO_Port, V2R_Pin, val & (1<<1) ? GPIO_PIN_SET : GPIO_PIN_RESET);
	HAL_GPIO_WritePin(V1R_GPIO_Port, V1R_Pin, val & (1<<0) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

//Sets the state of the RF switch (0 is normal operation, 1 is ADC measurement)
void set_switch(u8 val)
{
	if(val)
	{
		HAL_GPIO_WritePin(SV1_GPIO_Port, SV1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(SV2_GPIO_Port, SV2_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(SV1_GPIO_Port, SV1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(SV2_GPIO_Port, SV2_Pin, GPIO_PIN_RESET);
	}

}

//5-bit gain setting for differential amplifier
//See data sheet for gain definitions
void set_diff_gain(u8 val)
{
	//Pull CS and SCLK low
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
	HAL_Delay(1);
	int j = 0;
	for(int i = 0; i < 8; i++)
	{
		//If this is the last 3 bits then set them all to 0
		//if(i > 4)
		//{
		//	HAL_GPIO_WritePin(SDIO_GPIO_Port, SDIO_Pin, GPIO_PIN_RESET);
		//}
		//else//Otherwise start tapping out the specified gain
		//{
		HAL_GPIO_WritePin(SDIO_GPIO_Port, SDIO_Pin, val & (1 << i) ? GPIO_PIN_SET : GPIO_PIN_RESET);
		//}
		//Cycle the clock
		HAL_Delay(1);
		HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);
		HAL_Delay(1);
		if(j != 7)//If we're on the last clock cycle just go high
		{
			HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_RESET);
		}
		HAL_Delay(1);
		j++;
	}

	//Deselect CS
	HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
	//HAL_GPIO_WritePin(SCLK_GPIO_Port, SCLK_Pin, GPIO_PIN_SET);
}

//Enables or disables output amplifier
void set_amp_enable(u8 val)
{
	//If we're enabling
	if(val)
	{
		HAL_GPIO_WritePin(VA_GPIO_Port, VA_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(VB_GPIO_Port, VB_Pin, GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(VA_GPIO_Port, VA_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(VB_GPIO_Port, VB_Pin, GPIO_PIN_RESET);
	}
}

//If sel = 0, get ADC0 else ADC1
//Returns status (0 if OK) and returns read value via pass by reference
u8 read_adc(u8 sel, u16 *val)
{

	u8 ret_val = 0;

	ADC_ChannelConfTypeDef sConfig = {0};
	if(sel)
	{
		sConfig.Channel = ADC_CHANNEL_0;
	}
	else
	{
		sConfig.Channel = ADC_CHANNEL_1;
	}
	sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;
	if (HAL_ADC_ConfigChannel(&hadc, &sConfig) != HAL_OK)
	{
		ret_val = 1;
	}

	//Start the conversion
	HAL_ADC_Start(&hadc);
    //Poll ADC1 Perihperal & TimeOut = 1mSec
	HAL_ADC_PollForConversion(&hadc, 1);
	//Get the result and return it
	*val = HAL_ADC_GetValue(&hadc);
	HAL_ADC_Stop(&hadc);

	return ret_val;
}

//Returns status not result
//0 result is success
#define uart_buff_size 1024
u8 uart_rec_buff[16];//Buffer for UART hardware
u8 uart_buffer[uart_buff_size];//Our own buffer for storing incoming bytes
u16 uart_buffer_w_pos;
u16 uart_buffer_r_pos;
u16 uart_buffer_size;
void uart_init()
{
	//Initialize the buffer
	uart_buffer_w_pos = 0;
	uart_buffer_r_pos = 0;
	uart_buffer_size = 0;

	//Register the ISR callback here
	HAL_UART_Receive_IT(&huart2, uart_rec_buff, 1);
}

void  HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	//Copy the received byte into the local buffer
	uart_buffer[uart_buffer_w_pos] = uart_rec_buff[0];
	//Update the pointer
	uart_buffer_w_pos = uart_buffer_w_pos >= uart_buff_size-1 ? 0 : uart_buffer_w_pos + 1;
	//Update the size counter
	uart_buffer_size += 1;
	//Re-register the callback
	HAL_UART_Receive_IT(&huart2, uart_rec_buff, 1);
}

u8 get_uart_byte(u8 * res)
{
	//Old polling method
	//return HAL_UART_Receive(&huart2, res, 1, HAL_MAX_DELAY) == HAL_OK ? 0 : 1;

	//If the buffer is empty
	if(uart_buffer_size == 0)
	{
		//Return an error
		return 1;
	}

	//Set the return variable first
	*res = uart_buffer[uart_buffer_r_pos];

	//Update the read position pointer
	uart_buffer_r_pos = uart_buffer_r_pos >= uart_buff_size-1 ? 0 : uart_buffer_r_pos + 1;

	//Reduce the size counter
	uart_buffer_size -= 1;

	//Success
	return 0;
}




//Returns status not result
//0 result is success
u8 send_uart_byte(u8 * bt)
{
	while(HAL_UART_Transmit_IT(&huart2, bt, 1) == HAL_BUSY){}
	return 0;
}

void set_led(u8 num, u8 val)
{
	if(num)
	{
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, val ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
	else
	{
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, val ? GPIO_PIN_SET : GPIO_PIN_RESET);
	}
}

