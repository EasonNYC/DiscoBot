#ifndef RANDOM_NUMBER_GENERATOR_H
#define RANDOM_NUMBER_GENERATOR_H

#include <stdint.h>

void init_rng();  // initialize RNG
uint32_t get_random_number(); // generate one random number

#endif
