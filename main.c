#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

#include <stdio.h>
#include <math.h>

#include "accelerometers/accelerometers.h"
#include "temperature/temperature.h"
#include "RNG/random_number_generator.h"
#include "Task/timed_task.h"

#define NUMLETTERS 26

static void init_systick();
//static void delay_ms(uint32_t n);
volatile uint32_t msTicks;

//button state machine globals
typedef enum {ButtonIsPressed,ButtonIsReleased} _buttonstate;
volatile _buttonstate buttstate = ButtonIsReleased;
volatile uint32_t b_i;   //counts up when button is pressed (1 tick = 1ms)

//morse code stuff
typedef enum {ShortPress,LongPress,Wait,Fill} _pressType;
volatile _pressType presstype = Wait;
volatile uint32_t press_tick = 0;
volatile uint32_t release_tick = 0;

//morse letter symbol codes
uint8_t letters[26][4] = {
		{1,2,0,0},//A
		{2,1,1,1},
		{2,1,2,1},
		{2,1,1,0},//D
		{1,0,0,0},
		{1,1,2,1},
		{2,2,1,0},
		{1,1,1,1},
		{1,1,0,0},
		{1,2,2,2},
		{2,1,2,0},//K
		{1,2,1,1},
		{2,2,0,0},
		{2,1,0,0},
		{2,2,2,0},
		{1,2,2,1},
		{2,2,1,2},//Q
		{1,2,1,0},
		{1,1,1,0},
		{2,0,0,0},
		{1,1,2,0},
		{1,1,1,2}, //v
		{1,2,2,0},
		{2,1,1,2},
		{2,1,2,2},
		{2,2,1,1}};//z


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

void updatePressType(uint32_t presstime, uint32_t releasetime)
{
	if ( (releasetime - presstime) < 1500)
	{
		presstype = ShortPress;
	}
	else
	{
	    presstype = LongPress;
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
            //reset last press time
            press_tick = msTicks;

            break;

        case ButtonIsReleased:
            //printf("Button released\n");
            release_tick = msTicks;
            //check short/long press type
                updatePressType(press_tick,release_tick);
            break;

        }
    }
    else //we are holding the button down or the button is floating
    {
    	 if (((msTicks - release_tick) > 5000) && (buttstate == ButtonIsReleased))
		{
			presstype = Fill;
		}
		else
		{
			presstype = Wait;
		}

    }
    //save last state
    laststate = buttstate;
}

//handle press detected, set to none if greater than 5 secs?
void handlePress(uint8_t* arr, uint8_t arrsize, uint8_t* index)
{

//on startup
static volatile _pressType oldpress = Wait;

if (oldpress != presstype)
{
	//new event detected, switch case here
	switch (presstype)
	        {
				case ShortPress:
	                   printf("short press\n");
	              arr[*index] = 1;
	              	 ( *index)++;
	              break;

				case LongPress:
				        printf("long press\n");
				        arr[*index] =  2;
				        (*index)++;
						break;

				case Wait:
						//printf("pls press\n");
						//do nothing
						break;

				case Fill:
						printf("Filling...\n");
						for(uint8_t j = *index; j < 4; j++)
							arr[j] = 0;

						*index = 4;
						break;
	        }

}
//save last presstype
oldpress = presstype;
}

char findchar(uint8_t* curletter, uint8_t size)
{

	//for all letters in the array
	for(uint8_t i = 0; i < NUMLETTERS; i++) {

			//compare each symbol in a letter
			for(uint8_t j = 0; j < size; j++ ){
				if(curletter[j] != letters[i][j])
					break;
				if(j == 3)
					return 'A' + i;
			}

	}
	return 0;
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

funcptr callme = emptyfunc;
char chlookup[4] = {'A', 'T', 'M', 'R'};
funcptr flookup[4] = {printacc,
											printtemp,
											printtime,
											printRNG};


//main function to process morse code
void morse()
{
	//holding vars
	static uint8_t curletter[4] = {0,0,0,0};
	static uint8_t index = 0;

	//handle symbols
	if (index < 4){
	handlePress(curletter, 4, &index);
   }
   else //else lookup char and print if found
   {
    	char mychar = findchar(curletter,4);

    	for(int i =0; i < 4; i++)
    		printf("%d",curletter[i]);

    	if(mychar == 0) {
       		printf("-> unknown char\n");
    	}
      else {
       		printf("-> %c\n",mychar);
       		for(int i =0; i <4;i++){
       			if(mychar == chlookup[i]){
       				callme = flookup[i];
       			}
       		}
      }
    	//reset index etc.
    	index = 0;
    	for(int i =0; i < 4; i++)
    		curletter[i] =0;
    }
	}

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

int main(void)
{
  // initialize
  SystemInit();
  initialise_monitor_handles();
  init_systick();
  init_LED_pins();
  init_button();
  init_accelerometers(); // initialize accelerometers
  init_rng(); // initialize random number generator
  init_temperature_sensor();


  uint32_t t_prev = 0;
  while (1)
	{

	  //morse stuff
	  checkbutton();
	  morse();

    if ( (msTicks - t_prev) >= 1000) // 1 second has elapsed
    {
      t_prev = msTicks;

        callme();

    }
  }
}




