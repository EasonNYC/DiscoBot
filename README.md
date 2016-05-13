# DiscoBot
A Real Time Embedded STM32 Discovery robot. Stayin' Alive.

##Requirements
###Hardware
1. STM32-F4 ARM Cortex-M4 MCU (such as the STM32-F4 Discovery Board)    
2. 2 Xbee Radios
3. L293 Motor Driver
4. A Differential drive robot

**WIRING CONNECTIONS:**  

For PC controlled xbee:

| FTDI PIN  | XBEE PIN  |
| ------------ |:----------------:|
| GND          | GND              |
| 5V           | 5V              |
| RX          | TX              |
| TX          | RX              |
| PD0          | EOC              |

For robot Xbee:

| STM32F4 PIN  | XBEE  PIN  |
| ------------ |:----------------:|
| GND          | GND              |
| 3V           | 3V              |
| PB7          | TX              |
| PB6          | RX              |


###Software
-The ARM Standard Peripheral Library must be included to properly use this driver. Specifically make sure you have included these S.P.L. files in your project's working directory:  
  (update this list)
  "stm32f4xx.h"  
  "stm32f4xx_rcc.h"  
  "stm32f4xx_gpio.h"  
  "stm32f4xx_usart.h"  
  "misc.h"  

-Java Interface:

##Documentation:
This project is Doxygen compatible. A doxyfile has been included for easily generating documentation. Input and output paths may need to be edited to the path of your download directory.    

##Basic Usage:
(see: main.c for a working example)  

Open a terminal in the project directory and run the gnu make command:  
  make clean && make

Open a new terminal and run openocd: 
  openocd -f board/stm32f4discovery.cfg 

To flash the binary onto the STM32 board:
  st-flash write STM32_example.bin 0x8000000


