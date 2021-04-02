#include <stdio.h>
#include <arpa/inet.h>
#include <strings.h>
#include <unistd.h>
#include "sender.h"

int main(int argc, char *argv[]){


	if (argc < 2){
		return -1;
	}

	int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);

	
	struct sockaddr_in recipient;
	bzero(&recipient, sizeof(recipient));
	recipient.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &recipient.sin_addr);

	int ttl = 3;
	int pid = (int)getpid();
	
	setsockopt(sockfd, IPPROTO_ICMP, IP_TTL, &ttl, sizeof(int));
	send_icmp_packets(sockfd, recipient, pid, 0, 3);

		
	return 0;
}
