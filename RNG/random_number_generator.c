#include "random_number_generator.h"
#include <string.h>

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_rng.h"

// initialize RNG
void init_rng()
{
	/* Enable RNG clock source */
  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_RNG, ENABLE);
  /* RNG Peripheral enable */
  RNG_Cmd(ENABLE);
}

// generate one random number
uint32_t get_random_number()
{
	/* Wait until one RNG number is ready */
	while(RNG_GetFlagStatus(RNG_FLAG_DRDY)== RESET);
	/* Get a 32bit Random number */
	uint32_t r = RNG_GetRandomNumber();
	return r;
}
