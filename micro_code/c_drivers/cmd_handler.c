/*
 * cmd_handler.c
 *
 *  Created on: Jun 1, 2021
 *      Author: James Williams
 */

//Include for types
#include "hal_wrapper.h"
#include "cmd_handler.h"

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
#define CMD_SET_SWITCH 0x09

u8 state;
u8 cmd_byte;

#define STATE_PRE 0
#define STATE_CMD 1
#define STATE_DATA 2


void handler_entry()
{
	//Initialization
	uart_init();

	state = STATE_PRE;
	cmd_byte = 0;

	gpio_init();

	//handle command
	while(1)
	{
		handle_command();
	}
}


void handle_command()
{
	switch(state)//Do the FSM
	{
		case STATE_PRE:
			state_wait_preamble();
			break;
		case STATE_CMD:
			state_wait_cmd();
			break;
		case STATE_DATA:
			state_wait_data();
			break;
	}
	return;
}


void state_wait_preamble()
{
	//Wait for an incoming byte
	u8 pre_byte;
	//if there is no byte
	if(get_uart_byte(&pre_byte))
	{
		return;//Just try again
	}

	//If the preamble byte is bad
	if(pre_byte != BYTE_PREAMBLE)
	{
		return;//Try again
	}

	//Good preamble byte, move to next state
	state = STATE_CMD;
	return;
}

void state_wait_cmd()
{
	//Wait for the command byte
	if(get_uart_byte(&cmd_byte))
	{
		return;//abort if nothing available
	}

	//Command byte received, decode command
	//Switch the command
	u8 res = 0;
	u8 arg = 0;
	u16 adc_res;
	switch(cmd_byte)
	{

		case CMD_PING:
			//Send the ack and reset the state
			send_uart_byte(&res);
			state = STATE_PRE;
			break;

		case CMD_GET_ADC1:
			//Read ADC1's value
			read_adc(0, &adc_res);
			//Send it back over uart with msb first
			arg = (adc_res >> 8) & 0xff;
			send_uart_byte(&arg);
			arg = adc_res & 0xff;
			send_uart_byte(&arg);
			state = STATE_PRE;
			break;

		case CMD_GET_ADC2:
			//Read ADC1's value
			read_adc(1, &adc_res);
			//Send it back over uart with msb first
			arg = (adc_res >> 8) & 0xff;
			send_uart_byte(&arg);
			arg = adc_res & 0xff;
			send_uart_byte(&arg);
			state = STATE_PRE;
			break;

		default:
			//Must be a command that needs additional data
			state = STATE_DATA;
			break;

	}



}

void state_wait_data()
{
	//Wait for the argument byte
	u8 arg = 0;
	u8 res = 0;
	if(get_uart_byte(&arg)){return;}

	//Do something based on the command
	switch(cmd_byte)
	{
		case CMD_SET_LEFT:
			//Use it to set the left attenuation
			set_left_attenuation(arg);
			break;

		case CMD_SET_RIGHT:
			//Use it to set the right attenuation
			set_right_attenuation(arg);
			break;

		case CMD_SET_INPUT_GAIN:
			//Use it to set the input gain
			set_diff_gain(arg);
			break;

		case CMD_SET_OUTPUT_GAIN:
			//Use it to set the output gain (15dB or 0dB)
			set_amp_enable(arg);
			break;
		case CMD_SET_LED1:
			//set the LED state
			set_led(0, arg);
			break;

		case CMD_SET_LED2:
			//set the LED state
			set_led(1, arg);
			break;

		case CMD_SET_SWITCH:
			set_switch(arg);
			break;

	}
	//We've successfully executed the command so we can go back to waiting for the preamble
	send_uart_byte(&res);
	state = STATE_PRE;
	return;

}
