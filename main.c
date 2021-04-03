#include <stdio.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include "sender.h"
#include "reciver.h"


int main(int argc, char *argv[]){
	
	if (argc < 2){
		return -1;
	}
	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	
	struct sockaddr_in recipient;
	bzero(&recipient, sizeof(recipient));
	recipient.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &recipient.sin_addr);
	int ttl;
	int pid = (int)getpid();
	for(int i = 1; i<=30; i++){

		ttl = i;
		setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int));
		
		send_icmp_packets(sockfd, recipient, pid, 30, 3);
		printf("%d ", ttl);
		recive_icmp_packets(sockfd, pid, 30, 3, 1);
		printf("\n");
	}

		
	return 0;
}
