// Michał Zobniów 307275
#ifndef RECIVER
#define RECIVER

#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/time.h>
#include <stdbool.h>
#include <errno.h>
#include <time.h>

int recive_icmp_packets(int sockfd, int pid, int seq_num, int num_of_packets, int wait_time_in_seconds,
                        int *responses, char ip[][16], struct timeval timestamps[]);
double time_diff_in_seconds(struct timeval t1, struct timeval t2);
#endif
