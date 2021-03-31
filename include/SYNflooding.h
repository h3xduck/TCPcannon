#ifndef HEADER_SYNFLOOD
#define HEADER_SYNFLOOD


#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE 500
#endif

#include "../lib/RawTCP.h"
#include <stdio.h>
#include <signal.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdint.h>
#include <sys/time.h>

void syn_flooding_direct(const char* destination_address);

void syn_flooding_spoofed(const char* destination_address);

#endif