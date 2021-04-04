// Michał Zobniów 307275
#ifndef SENDER
#define SENDER

#include <stdlib.h>
#include <assert.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>

int send_icmp_packets(int sockdf, struct sockaddr_in recipient, int pid, int seq_num, int num_of_packets);

#endif
