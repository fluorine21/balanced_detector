/*
 * cmd_handler.c
 *
 *  Created on: Jun 1, 2021
 *      Author: James Williams
 */

//Include for types
#include "hal_wrapper.h"

//Byte definitions
#define BYTE_PREAMBLE 0xAA

//Command definitions
#define CMD_PING 0x00
#define CMD_SET_LEFT 0x01
#define CMD_SET_RIGHT 0x02
#define CMD_SET_INPUT_GAIN 0x03
#define CMD_SET_OUTPUT_GAIN 0x04
#define CMD_GET_ADC1 0x05
#define CMD_GET_ADC2 0x06
#define CMD_SET_LED1 0x07
#define CMD_SET_LED2 0x08


void handle_command();

void handler_entry()
{
	//Initialization



	//handle command
	while(1)
	{
		handle_command();
	}
}


void handle_command()
{
	//Wait for an incoming byte
	u8 pre_byte;
	//if HAL returns an error here
	if(get_uart_byte(&pre_byte))
	{
		return;//Just try again
	}

	//If the preamble byte is bad
	if(pre_byte != BYTE_PREAMBLE)
	{
		return;//Try again
	}

	//Wait for the command byte
	u8 cmd_byte;
	if(get_uart_byte(&cmd_byte))
	{
		return;//abort if reception fails
	}

	//Switch the command
	u8 res = 0;
	u8 arg = 0;
	switch(cmd_byte)
	{

		case CMD_PING:
			//Do nothing, by default ack byte is sent at end
			break;

		case CMD_SET_LEFT:
			//Get the next byte
			if(get_uart_byte(arg)){return;}
			//Use it to set the left attenuation
			set_left_attenuation(arg);
			break;

		case CMD_SET_RIGHT:

			break;

		case CMD_SET_INPUT_GAIN:

			break;

		case CMD_SET_OUTPUT_GAIN:

			break;

		case CMD_GET_ADC1:

			break;

		case CMD_GET_ADC2:

			break;

		case CMD_SET_LED1:

			break;

		case CMD_SET_LED2:

			break;

	}
	//Send the ack
	send_uart_byte(res);
	return;
}
