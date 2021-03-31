#ifndef HEADER_CWMC
#define HEADER_CWMC

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdint.h>

/**
 * CWMC - Multiply with Carry random number generator
 * 
 * Algorithm from https://en.wikipedia.org/wiki/Multiply-with-carry_pseudorandom_number_generator
 * 
 */ 

#define LOG_1(n) (((n) >= 2) ? 1 : 0)
#define LOG_2(n) (((n) >= 1<<2) ? (2 + LOG_1((n)>>2)) : LOG_1(n))
#define LOG_4(n) (((n) >= 1<<4) ? (4 + LOG_2((n)>>4)) : LOG_2(n))
#define LOG_8(n) (((n) >= 1<<8) ? (8 + LOG_4((n)>>8)) : LOG_4(n))
#define LOG(n)   (((n) >= 1<<16) ? (16 + LOG_8((n)>>16)) : LOG_8(n))
#define BITS_TO_REPRESENT(n) (LOG(n) + !!((n) & ((n) - 1)))
#if ((RAND_MAX | (RAND_MAX >> 1)) != RAND_MAX) 
#error "expected a RAND_MAX that is 2^n - 1"
#endif
#define RAND_BITS BITS_TO_REPRESENT(RAND_MAX)

// CMWC working parts
#define CMWC_CYCLE 4096         
#define CMWC_C_MAX 809430660    
struct cmwc_state {
	uint32_t Q[CMWC_CYCLE];
	uint32_t c;	// must be limited with CMWC_C_MAX
	unsigned i;
};

uint32_t rand32(void);

void initCMWC(struct cmwc_state *state, unsigned int seed);

uint32_t randCMWC(struct cmwc_state *state);

#endif