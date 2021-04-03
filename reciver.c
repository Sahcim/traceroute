#include "reciver.h" 

int recive_icmp_packets(int sockfd, int pid, int seq_start, int num_of_packets, int wait_time_in_seconds){

	clock_t start;
	start = clock();
	int responses = 0; // number_of_recived packets
	clock_t timestamps[num_of_packets]; // times of packets reciving 	
	char ip[num_of_packets][30]; // storage of ips	
	// Select setup
	int select_status;
	struct timeval tv; tv.tv_sec = wait_time_in_seconds; tv.tv_usec = 0; // timeout time
	fd_set descriptors;
	FD_ZERO(&descriptors);
	FD_SET(sockfd, &descriptors);
	// recive setup
	struct sockaddr_in sender;
	socklen_t sender_len =sizeof(sender);
	u_int8_t buffer[IP_MAXPACKET];
	// Packet unpacking variables
	struct ip* ip_header;
	struct icmp* icmp_header;
	struct in_addr* response_ip;
	
	while (responses < num_of_packets){
		select_status = select(sockfd+1, &descriptors, NULL, NULL, &tv);
		if (select_status < 0){
			return -1;
		}
		else if (select_status == 0){			
			break;
		}
		else{
			recvfrom(sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len);

			ip_header = (struct ip*) buffer;
			icmp_header = (struct icmp*) (buffer + 4 * ip_header->ip_hl);
			response_ip = &ip_header -> ip_src;
			
			if (icmp_header -> icmp_type == ICMP_TIME_EXCEEDED || icmp_header -> icmp_type == ICMP_ECHOREPLY){
				if (icmp_header -> icmp_type == ICMP_TIME_EXCEEDED){
					ip_header = (struct ip *)((uint8_t *)icmp_header + 8);			
					icmp_header = (struct icmp *) ((uint8_t *) ip_header + ip_header->ip_hl * 4);
				}
				
				if (seq_start <= icmp_header -> icmp_seq && 
								icmp_header -> icmp_seq  <= seq_start+num_of_packets &&
								icmp_header -> icmp_id==pid){
					inet_ntop(AF_INET, response_ip, ip[responses], sizeof(char[30]));
					timestamps[responses] = clock();
					responses++;
				}			
			}	
		}
	}

	for (int i = 0; i<responses; i++){
			int msec = (int)(timestamps[i]-start)/CLOCKS_PER_SEC*1000;
			printf("%s %d", ip[i], msec % 1000);
	}

	return 1;
}
