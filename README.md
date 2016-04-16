# DiscoBot
A Real Time Embedded disco robot. Stayin' Alive.

##Requirements
###Hardware
1. STM32-F4 ARM Cortex-M4 MCU (such as the STM32-F4 Discovery Board)    
2. 2 Xbee Radios
3. L293 Motor Driver
4. Differential drive robot

**WIRING CONNECTIONS:**  


| STM32F4 PIN  | Peripheral  PIN  |
| ------------ |:----------------:|
| GND          | GND              |
| 5V           | VIN              |
| PB7          | SDA              |
| PB6          | SCL              |
| PD0          | EOC              |


###Software
-The ARM Standard Peripheral Library must be included to properly use this driver. Specifically make sure you have included these S.P.L. files in your project's working directory:  
  (update this list)
  "stm32f4xx.h"  
  "stm32f4xx_i2c.h"  
  "stm32f4xx_rcc.h"  
  "stm32f4xx_gpio.h"  
  "stm32f4xx_syscfg.h"  
  "stm32f4xx_exti.h"  

##Documentation:
All files have been commented and are Doxygen compatible. A doxyfile has been included for easily generating documentation. Input and output paths may need to be edited to the path of your download directory.    

##Basic Usage:
(see: main.c for a working example)  
