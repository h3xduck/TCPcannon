#include "../include/iputils.h"

int FLAG_RANDOM_GENERATOR_INITIALIZED = 0;
struct cmwc_state cmwc;

const char* generate_random_address(){
    if(FLAG_RANDOM_GENERATOR_INITIALIZED == 0){
        unsigned int seed = time(NULL);
	    initCMWC(&cmwc, seed);
        FLAG_RANDOM_GENERATOR_INITIALIZED = 1;
    }
	
	uint32_t random_num = randCMWC(&cmwc);

    uint32_t result;
    result = (random_num >> 24 & 0xFF) << 24 | 
            (random_num >> 16 & 0xFF) << 16 | 
            (random_num >> 8 & 0xFF) << 8 | 
            (random_num & 0xFF);

    char *address = malloc(sizeof(char)*32);
    inet_ntop(AF_INET, (void*)&result, address, 32);

    return address;
}
