/*
 * Copyright (c) 2009 Xilinx, Inc.  All rights reserved.
 *
 * Xilinx, Inc.
 * XILINX IS PROVIDING THIS DESIGN, CODE, OR INFORMATION "AS IS" AS A
 * COURTESY TO YOU.  BY PROVIDING THIS DESIGN, CODE, OR INFORMATION AS
 * ONE POSSIBLE   IMPLEMENTATION OF THIS FEATURE, APPLICATION OR
 * STANDARD, XILINX IS MAKING NO REPRESENTATION THAT THIS IMPLEMENTATION
 * IS FREE FROM ANY CLAIMS OF INFRINGEMENT, AND YOU ARE RESPONSIBLE
 * FOR OBTAINING ANY RIGHTS YOU MAY REQUIRE FOR YOUR IMPLEMENTATION.
 * XILINX EXPRESSLY DISCLAIMS ANY WARRANTY WHATSOEVER WITH RESPECT TO
 * THE ADEQUACY OF THE IMPLEMENTATION, INCLUDING BUT NOT LIMITED TO
 * ANY WARRANTIES OR REPRESENTATIONS THAT THIS IMPLEMENTATION IS FREE
 * FROM CLAIMS OF INFRINGEMENT, IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */

/*
 * helloworld.c: simple test application
 */

#include <stdio.h>
#include "platform.h"
#include "mblaze_nt_types.h"
#include "xparameters.h"
#include "xgpio.h"
#include "math.h"

void print(char *str);

int main()
{	uint16_t up; //2 = UP 1 = STOP 0 = DOWN
	uint16_t down;
	int going_right;
	uint32_t bat_desired_center = 0;
	uint32_t ball_x = 0;
	uint32_t ball_y = 0;
	uint32_t bat_l = 0;
	uint32_t speed = 0;
	uint32_t last_ball_pos = 0;
	uint32_t last_ball_pos_y = 0;
	volatile uint32_t* ptr_pushButtons = ((volatile uint32_t*)(XPAR_PUSH_BUTTONS_3BIT_BASEADDR));
	volatile uint32_t* ptr_LEDs = ((volatile uint32_t*)(XPAR_LEDS_8BIT_BASEADDR));
	volatile uint32_t* ptr_ballColor = ((volatile uint32_t*)(XPAR_BALL_COLOR_O_BASEADDR));
	volatile uint32_t* ptr_bat_pos = ((volatile uint32_t*)(XPAR_BATPOS_L_R_BASEADDR));
	volatile uint32_t* ptr_ball_pos = ((volatile uint32_t*)(XPAR_BALLPOS_XY_BASEADDR));

    init_platform();

    while(1){

       	  ball_y = *ptr_ball_pos;
    	  ball_y &= 0x3ff;
    	  ball_x = *ptr_ball_pos<<10;
		  ball_x &= 0x3ff;
		  bat_l = *ptr_bat_pos<<10;
		  bat_l &= 0x3ff;

    	  last_ball_pos = ball_x;
    	  last_ball_pos_y = ball_y;

    	  if (last_ball_pos > (ball_x + 1)) {
    		  if (going_right == 0) {
    			  *ptr_ballColor += 1;
    		  }
    		  going_right = 1;
    	  }
    	  else if (last_ball_pos < (ball_x - 1)) {
    		  if (going_right == 1) {
    			  *ptr_ballColor += 1;
    		  }
    		  going_right = 0;
    	  }

    	  if (last_ball_pos >= ball_x) {
    		  if (last_ball_pos_y >= ball_y) {
    			  speed = (last_ball_pos - ball_x) + (last_ball_pos_y - ball_y) ;
    		  }
    		  else {
    			  speed = (last_ball_pos - ball_x) + (ball_y - last_ball_pos_y) ;
    		  }

    	  }
    	  else {
    		  if (last_ball_pos_y <= ball_y) {
				  speed = (ball_x - last_ball_pos) + (last_ball_pos_y - ball_y) ;
			  }
			  else {
				  speed = (ball_x - last_ball_pos) + (ball_y - last_ball_pos_y) ;
			  }

    	  }
    	  bat_desired_center = ball_y+10;
    	  if (bat_desired_center > (bat_l+50)) {
    		  if (bat_l > 379) {
    			  up = 0; // STOP
    			  down = 0;
    		  }
    		  else {
    			  down = 1; //GO DOWN
    		  }
    	  }
    	  else if (bat_desired_center < (bat_l+50)) {
    		  if (bat_l < 1) {
    			  up = 0; //STOP
    			  down = 0;
    		  }
    		  else {
    			  up = 1; //GO UP
    		  }
    	  }

    	// Set LEDs according to push buttons
    	*ptr_LEDs = *ptr_pushButtons;

    	// Delay
    	//for ( i = 0;  i < 500000; ++ i);
    }

    return 0;
}
