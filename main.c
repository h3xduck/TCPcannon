
#include "include/SYNflooding.h"
#include "include/benchmarks.h"

int main(){

    syn_flooding_direct("192.168.1.1");
    //benchmark_syn_flooding_spoofed("192.168.1.1");

    return 0;
}