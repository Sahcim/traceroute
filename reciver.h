#ifndef RECIVER
#define RECIVER

#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <time.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>


int recive_icmp_packets(int sockfd, int pid, int seq_start, int num_of_packets, int wait_time_in_seconds);

#endif
