#ifndef CAR_H
#define CAR_H


/*
this enum will be used by set_left_motor_direc and
set_right_motor_direc
*/
typedef enum
{
  FORWARD,
  BACKWARD,
  STOP

} Directions;



void init_GPIO_A1A2A3A4_output();
void set_left_motor_direc(Directions direc, float speed);
void set_right_motor_direc(Directions direc, float speed);

void move_forward();
void move_backward();
void move_forward_soft_left();
void move_forward_soft_right();
void move_backward_soft_left();
void move_backward_soft_right();
void move_spin_right();
void move_spin_left();
void stop();


#endif
