/*
 * ax.c
 *
 *  Created on: Nov 7, 2017
 *      Author: silardg
 */

#include <stdio.h>
#include "ax.h"
#include "dynamixel.h"

static uint16_t ax_convert_angle_to_10(float angle)   		   	{ return (uint16_t)(1023 * angle) / 300;     } // converts angle(0-300) to register range (0-1023)
static float	ax_convert_10_to_angle(uint16_t angle_bit) 		{ return (float)(angle_bit * 300) / 1023;    } // converts register range (0-1023) to angle(0-300)
static uint16_t ax_convert_speed_to_10(uint8_t speed)	   		{ return (uint16_t)(1023 * speed) / 100;	 } // converts speed (0-100) to register range (0-1023)
static uint8_t  ax_convert_10_to_speed(uint16_t speed_bit) 		{ return (uint8_t)(speed_bit * 100) / 1023;  } // converts register range (0-1023) to speed (0-100)

/*
 * 	Function:    uint8_t ax_set_angle(uint8_t id, float angle, uint8_t speed)
 * 	Paramters:   uint8_t id    -> the id of the servo
 * 				 float angle   -> the angle (0-300) in decimal
 * 				 uint8_t speed -> the speed of the servo (0-100)
 * 	Description: it sets the angle and the speed of the given id servo
 */
uint8_t ax_set_angle(uint8_t id, float angle, uint8_t speed) {

	uint16_t ax_angle = ax_convert_angle_to_10(angle);	// converts the angle
	uint16_t ax_speed = ax_convert_speed_to_10(speed);  // converts the speed

	// writes to dynamixel and returns a 1 if everything is okay
	if(dynamixel_writeword(id, AX_GOAL_SPEED_L, ax_speed) == DYNAMIXEL_SUCCESS && dynamixel_writeword(id, AX_GOAL_POSITION_L, ax_angle) == DYNAMIXEL_SUCCESS)
		return 1; // everything okay
	else
		return 0; // some error
}

/*
 * 	Function:    uint8_t ax_get_speed(uint8_t id)
 * 	Parameters:  uint8_t id -> the id of the servo
 * 	Description: returns the present speed that the servo is moving
 */
uint8_t ax_get_speed(uint8_t id) {

	uint16_t present_speed;	// holder for the value

	if(dynamixel_readword(id, AX_PRESENT_SPEED_L, &present_speed) == DYNAMIXEL_SUCCESS)
		return ax_convert_10_to_speed(present_speed); // converts the range to 0-100 from the register IF everything is okay
	else
		return 2; // some error
}

/*
 * 	Function:    float ax_get_angle(uint8_t id)
 * 	Parameters:  uint8_t id -> the id of the servo
 * 	Description: returns the current angle of the servo
 */
float ax_get_angle(uint8_t id) {

	uint16_t present_angle; // holder for the value

	if(dynamixel_readword(id, AX_PRESENT_POSITION_L, &present_angle) == DYNAMIXEL_SUCCESS)
		return ax_convert_10_to_angle(present_angle); // converts the range to 0-300 from the register IF everything is okay
	else
		return 2; // some error
}

/*
 * 	Function:    uint8_t ax_check_moving(uint8_t id)
 * 	Parameters:  uint8_t id -> the id of the servo
 * 	Description: returns the status of the moving of the servo (0 or 1)
 */
uint8_t ax_check_moving(uint8_t id) {

	uint8_t moving_status; // holder for the value

	if(dynamixel_readbyte(id, AX_MOVING, &moving_status) == DYNAMIXEL_SUCCESS)
		return moving_status; // returns the value of the moving register IF everything is okay
	else
		return 2; // some error
}

/*
 * 	Function:    uint8_t ax_check_temperature(uint8_t id)
 * 	Parameters:  uint8_t id -> the id of the servo
 * 	Description: returns the temperature of the servo in celsius
 */
uint8_t ax_check_temperature(uint8_t id) {

	uint8_t temperature_status; // holder for the value

	if(dynamixel_readbyte(id, AX_PRESENT_VOLTAGE, &temperature_status) == DYNAMIXEL_SUCCESS)
		return temperature_status; // returns the temperature of the servo IF everything is okay
	else
		return 0; // some error
}

/*
 * 	Function:    uint8_t ax_check_voltage(uint8_t id)
 * 	Parameters:  uint8_t id -> the id of the servo
 * 	Description: returns the voltage on the servo
 */
uint8_t ax_check_voltage(uint8_t id) {

	uint8_t voltage_status; // holder for the value

	if(dynamixel_readbyte(id, AX_PRESENT_VOLTAGE, &voltage_status) == DYNAMIXEL_SUCCESS)
		return voltage_status / 10; // it returns the 10* voltage of the servo IF everything is okay
	else
		return 0; // some error
}
