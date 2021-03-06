/*
 * cmd_handler.h
 *
 *  Created on: Jun 29, 2021
 *      Author: James Williams
 */

#ifndef APPLICATION_USER_CORE_C_DRIVERS_CMD_HANDLER_H_
#define APPLICATION_USER_CORE_C_DRIVERS_CMD_HANDLER_H_


void handler_entry();
void handle_command();
void state_wait_preamble();
void state_wait_cmd();
void state_wait_data();

#endif /* APPLICATION_USER_CORE_C_DRIVERS_CMD_HANDLER_H_ */
