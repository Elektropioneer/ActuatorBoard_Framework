/*
 * ax.c
 *
 *  Created on: Nov 7, 2017
 *      Author: silardg
 */

#include "ax.h"
#include "dynamixel.h"

uint8_t ax_set_angle(uint8_t id, uint16_t angle, uint16_t speed) {
	if(dynamixel_writeword(id, AX_GOAL_SPEED_L, speed) == DYNAMIXEL_SUCCESS && dynamixel_writeword(id, AX_GOAL_POSITION_L, angle) == DYNAMIXEL_SUCCESS)
		return 1;
	else
		return 0;
}

uint16_t ax_get_angle(uint8_t id) {
	uint16_t present_angle;
	if(dynamixel_readword(id, AX_PRESENT_POSITION_L, &present_angle) == DYNAMIXEL_SUCCESS)
		return present_angle;
	else
		return 0;
}

uint8_t ax_check_moving(uint8_t id) {
	uint8_t moving_status;
	if(dynamixel_readbyte(id, AX_MOVING, &moving_status) == DYNAMIXEL_SUCCESS)
		return moving_status;
	else
		return 0;
}
