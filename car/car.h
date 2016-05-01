#ifndef CAR_H
#define CAR_H


void init_car_GPIOB();
void set_left_motor_direc(uint32_t direc, float speed);

typedef enum
{
  FORWARD,
  BACKWARD,
  STOP

} Directions;


#endif
