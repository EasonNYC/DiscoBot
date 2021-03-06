#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#include <stdio.h>
#include <math.h>

#include "accelerometers/accelerometers.h"
#include "temperature/temperature.h"
#include "RNG/random_number_generator.h"
#include "Task/timed_task.h"
#include "usart/usart.h"
#include "car/car.h"

//#include "circarray/circarray.h"

static void init_systick();
//static void delay_ms(uint32_t n);
volatile uint32_t msTicks;

//button state machine globals
typedef enum {ButtonIsPressed,ButtonIsReleased} _buttonstate;
volatile _buttonstate buttstate = ButtonIsReleased;
volatile uint32_t b_i;   //counts up when button is pressed (1 tick = 1ms)

//reads the button on the discovery board
uint32_t read_buttonc( uint32_t i )
{
//check for errors
if(i > 3) return -1;

//read button
return (GPIOA->IDR >> i) & 1;

}

// SysTick Handler (Interrupt Service Routine for the System Tick interrupt)
void SysTick_Handler(void){
  msTicks++;

  //button press logic / state machine
  if(read_buttonc(0))
  {

      if((buttstate == ButtonIsReleased) && (b_i >= 250))
      {
      buttstate = ButtonIsPressed;
      }

  b_i++; //increment button counter

  }
  else
  {
      //reset counter
       b_i = 0;
       buttstate = ButtonIsReleased;
  }
}

//on button change state machine
void checkbutton(void)
{
    static _buttonstate laststate = ButtonIsReleased;

    if (laststate != buttstate)
    {
        switch (buttstate)
        {

        case ButtonIsPressed:
            //printf("Button pressed\n");
             //do something
            break;

        case ButtonIsReleased:
            //printf("Button released\n");
            //do something else
            break;

        }
    }

    //save last state
    laststate = buttstate;
}

//function lookup
typedef void (*funcptr)(void);

//functions to point to
void printacc(void){
   float a[3]; // array of 3 floats into which accelerometer data will be read
   read_accelerometers(a); // read data from accelerometers (X, Y, and Z axes)
    printf("%f %f %f\n", a[0], a[1], a[2]);
    float pitch, roll;
    calc_pitch_roll(a[0], a[1], a[2], &pitch, &roll);
    printf("Pitch = %f, Roll = %f\n", pitch, roll);

}
void printtemp(void){
  float temp = read_temperature_sensor();
  printf("Temp = %f\n",temp);
}
void printtime(void){
printf("msTicks = %d\n",msTicks);
}
void printRNG(void){
   uint32_t r = get_random_number();
    printf("Random = %lu\n",r); // print unsigned int
}
void emptyfunc(void){
  //for initialization in main loop
}

//funcptr callme = emptyfunc;
char chlookup[4] = {'A', 'T', 'M', 'R'};
/*funcptr flookup[4] = {printacc,
                      printtemp,
                      printtime,
                      printRNG};
*/


// initialize the system tick
void init_systick(void)
{
  SystemCoreClockUpdate();                      /* Get Core Clock Frequency   */
  if (SysTick_Config(SystemCoreClock / 1000)) { /* SysTick 1 msec interrupts  */
    while (1);                                  /* Capture error              */
  }
}

// pause for a specified number (n) of milliseconds
/*
void delay_ms(uint32_t n)
{
  uint32_t msTicks2 = msTicks + n;
  while(msTicks < msTicks2) ;
}
*/

void init_LED_pins()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;  // enable clock to GPIOD

  for (int i=12; i<=15; i++)
  {
    GPIOD->MODER &= ~(0x3 << (2*i)); // clear the 2 bits corresponding to pin i
    GPIOD->MODER |= (1 << (2*i));    // set pin i to be general purpose output
  }
}

void LED_On(uint32_t i)
{
  GPIOD->BSRRL = 1 << (i+12);
}

void LED_Off(uint32_t i)
{
  GPIOD->BSRRH = 1 << (i+12);
}


void init_button()
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;  // enable clock to GPIOA

  GPIOA->MODER &= ~(0x3 << (2*0)); // clear the 2 bits corresponding to pin 0
  // if the 2 bits corresponding to pin 0 are 00, then it is in input mode
}

void calc_pitch_roll(float acc_x, float acc_y, float acc_z, float *pitch, float *roll)
{
  *roll = (180.0/M_PI)*atan2(acc_y, acc_z);
  *pitch = (180.0/M_PI)*atan2(-acc_x, sqrt(acc_y*acc_y+acc_z*acc_z));
}

void initialise_monitor_handles();

//incoming message id (used with funcptr array of motor commands)
typedef enum
  {
    FWD = 0,
    BWD =1,
    FWDLEFT =2,
  FWDRIGHT =3,
  BCKLEFT =4,
  BCKRIGHT =5,
  SPINRIGHT =6,
  SPINLEFT =7,
  STOPCAR = 8,
  MAXIDSIZE
  } _ID;

  _ID msgid = STOPCAR; //msgid 8 (stop)

  funcptr flookup[MAXIDSIZE] = { //called with callme in main while loop
      move_forward, //msgid = 0
      move_backward,
      move_forward_soft_left,
      move_forward_soft_right,
      move_backward_soft_left,
      move_backward_soft_right,
      move_spin_right,
      move_spin_left,
      stop //msgid = 8
  };

  funcptr callme = stop; //first command to be called on startup is stop


//incoming messages will be like '$'+'MSGID'+'*'
//so the sequence $8* would be the message from java to call the stop function

int main(void)
{
  // initialize
  SystemInit();
  // initialise_monitor_handles();
  init_systick();
  init_LED_pins();
  init_button();
  init_accelerometers(); // initialize accelerometers
  init_rng(); // initialize random number generator
  init_temperature_sensor();

  //init gpio for lm293 init
  init_GPIO_A1A2A3A4_output();

  //usart init
  init_usart1(9600);
  usart1_send( "UART1 Initialized. @9600bps\r\n");


  uint32_t t_prev = 0;
  while (1)
  {

    //check serial buffer for 3 incoming bytes
    while(usart1_available() > 0){ //if waiting for 5 bytes, might put avail() > 5 etc.
     //char mybyte =  (char)usart1_read(); //get/store next byte from buffer in a variable to do something with
      //printf("%c",usart1_readc()); //print as a char the next byte available to read from buffer

      //parse incoming message
      // printf("received\n");
      char read = usart1_readc();

      if (read >= 0 && read <= 8) {
        callme = flookup[read]; //assign function/motor command to be called by callme using rxid as index
        callme();

        // printf("received %c", read);
      }

      /*
      char last = 0;
      uint8_t rxid;
      */
      //printf("received %c", read);

      /*
      if (received == '*') //if start byte (should be $)
      {
        printf("start correct\n");
        rxid = usart1_read(); //next byte is msgID byte
        //rxid -= 48; //convert from ascii to regular number
        last = usart1_readc(); //get last byte (should be' *')
        printf("rxid: %d last: %d\n", rxid,last);
      }
      else
      {
        printf("b start: %d\n", start);
        usart1_read();

        break; //bad message (discard incoming chars till it finds a $ symbol or buffer is empty)

      }

       if (last == '*') //its a good message
      {
        printf("made itcallme, rxid: %d\n", rxid);

      callme = flookup[rxid]; //assign function/motor command to be called by callme using rxid as index
      while(usart1_available())
      {
      usart1_read();//throw away any leftover
      }
      }
      */
    }

  //robot stuff

  //callme(); //runs received wireless motor commands

  //below may not be nessisary anymore
  //  set_left_motor_direc(FORWARD,0);
  //  set_right_motor_direc(BACKWARD,0);
  //  move_forward();
  //  delay_ms(3000);
  //  // set_right_motor_direc(FORWARD,0);
  //  // set_left_motor_direc(BACKWARD,0);
  //  move_backward();
  //  delay_ms(3000);
  //  set_right_motor_direc(STOP,0);
  //  set_left_motor_direc(STOP,0);
  //  delay_ms(3000);

   //  //checkbutton();


   //timed task stuff
     if ((msTicks - t_prev) > 1000)
    {

       //send outgoing messages to java here
       //somesendfunction(message)

      //  printf("rx: %d\n", usart1_available()); //keep in for now. makes sure serial data coming through while loop is not optimized out.

       /*
       if (usart1_available() == 1)
       {
         uint8_t thechar = usart1_peek_byte();
         printf("buf: %d", thechar );

       }
       */
      t_prev = msTicks;
    }

  }
}
