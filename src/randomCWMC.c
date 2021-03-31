#include "../include/randomCWMC.h"

// Collect 32 bits of rand(). You are encouraged to use a better source instead.
uint32_t rand32(void){
	uint32_t result = rand();
    for (int bits = RAND_BITS; bits < 32; bits += RAND_BITS){
        result = result << RAND_BITS | rand();
    }
	return result;
}

// Init the state with seed
void initCMWC(struct cmwc_state *state, unsigned int seed){
	srand(seed);        
	for (int i = 0; i < CMWC_CYCLE; i++)
		state->Q[i] = rand32();
	do
		state->c = rand32();
	while (state->c >= CMWC_C_MAX);
	state->i = CMWC_CYCLE - 1;
}

// CMWC engine
uint32_t randCMWC(struct cmwc_state *state){  //EDITED parameter *state was missing
	uint64_t const a = 18782;	// as Marsaglia recommends
	uint32_t const m = 0xfffffffe;	// as Marsaglia recommends
	uint64_t t;
	uint32_t x;

	state->i = (state->i + 1) & (CMWC_CYCLE - 1);
	t = a * state->Q[state->i] + state->c;
	/* Let c = t / 0xffffffff, x = t mod 0xffffffff */
	state->c = t >> 32;
	x = t + state->c;
	if (x < state->c) {
		x++;
		state->c++;
	}
	return state->Q[state->i] = m - x;
}