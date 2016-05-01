#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#include <stdio.h>
#include "car/car.h"

/*
intialise GpioA 1-4 as output for the connection to the LM293d for
motor direction of Left and Right motors
*/
void init_GPIO_A1A2A3A4_output()
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,  ENABLE); // enable clock for GPIOA
	GPIO_InitStruct.GPIO_OType 		= GPIO_OType_PP; // configure as push-pull
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT; // configure as outputs
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; // pull-up (logic 1 when floating)
	GPIO_InitStruct.GPIO_Pin 			= GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 ; // choose pins 1 and 2 (i.e., A1 and A2)
	GPIO_InitStruct.GPIO_Speed 		= GPIO_Speed_25MHz; // set max update speed to 25 MHz
	GPIO_Init( GPIOA, &GPIO_InitStruct); // initialize
}

/*
low level function to set left motor direction.
Will be used by functions go_forward, go_backward etc.
Using GPIOA 1&2 which
are connected to the motor input pins of the LM293D motor driver
which correspond to the left motor
argument direc: Possbilities are FORWARD, BACKWARD or STOP
argument float: speed that will be the duty cycle of the enable pin
haven't implemented yet
*/
void set_left_motor_direc(Directions direc, float speed)
{
  if (direc == FORWARD)
  {
    GPIO_WriteBit(GPIOA,GPIO_Pin_1,1);
    GPIO_WriteBit(GPIOA,GPIO_Pin_2,0);
  }
  else if (direc == BACKWARD)
  {
    GPIO_WriteBit(GPIOA,GPIO_Pin_1,0);
    GPIO_WriteBit(GPIOA,GPIO_Pin_2,1);
  }
  else
  {
    GPIO_WriteBit(GPIOA,GPIO_Pin_1,0);
    GPIO_WriteBit(GPIOA,GPIO_Pin_2,0);
  }

}

/*
low level function to set right motor direction. Using GPIOA 3&4 which
are connected to the motor input pins of the LM293D motor driver
which correspond to the right motor
argument direc: Possbilities are FORWARD, BACKWARD or STOP
argument float: speed that will be the duty cycle of the enable pin
*/

void set_right_motor_direc(Directions direc, float speed)
{
  if (direc == FORWARD)
  {
    GPIO_WriteBit(GPIOA,GPIO_Pin_3,1);
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,0);
  }
  else if (direc == BACKWARD)
  {
    GPIO_WriteBit(GPIOA,GPIO_Pin_3,0);
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,1);
  }
  else
  {
    GPIO_WriteBit(GPIOA,GPIO_Pin_3,0);
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,0);
  }

}

/*
*********************************************
Below are 9 functions that control the direction of the robot
Here is  a diagram of 6 of the directions
   |
 \   /
 /   \
   |
   this includes:
move_forward() and move_backward() are as they sound
move_forward_soft_left()
move_forward_soft_right()
move_backward_soft_left()
move_backward_soft_right()

then there are 3 more functions:
stop() is self explanatory
then there is spin in place to the left ("hard left")
and spin to the right (hard right)
*********************************************
*/



/*
function to make robot go forward indefinitely
havent set up PWM yet the motors currently run at full speed
*/
void move_forward()
{
  set_left_motor_direc(FORWARD,0);
  set_right_motor_direc(FORWARD,0);

}

/*
make robot move backward indefinitely
havent set up PWM yet the motors currently run at full speed
*/
void move_backward()
{
  set_left_motor_direc(BACKWARD,0);
  set_right_motor_direc(BACKWARD,0);

}

void move_forward_soft_left()
{
}


void move_forward_soft_right()
{

}


void move_backward_soft_left()
{

}

void move_backward_soft_right()
{

}


void move_spin_right()
{

}

void move_spin_left()
{

}


void stop()
{

}
