#include "reciver.h" 

int recive_icmp_packets(int sockfd, int pid, int seq_start, int num_of_packets, int wait_time_in_seconds){

	fd_set descriptors;
	int responses = 0;
	int select_status;
	struct timeval tv; tv.tv_sec = wait_time_in_seconds; tv.tv_usec = 0;
	struct sockaddr_in sender;
	socklen_t sender_len =sizeof(sender);
	u_int8_t buffer[IP_MAXPACKET];

	FD_ZERO(&descriptors);
	FD_SET(sockfd, &descriptors);

	struct ip* ip_header;
	struct icmp* icmp_header;
	struct in_addr* r_ip;
	
	while (responses < num_of_packets){
		select_status = select(sockfd+1, &descriptors, NULL, NULL, &tv);
		if (select_status < 0){
			return -1;
		}
		else if (select_status == 0){			
			return responses;
		}
		else{
			recvfrom(sockfd, buffer, IP_MAXPACKET, MSG_DONTWAIT, (struct sockaddr*)&sender, &sender_len);

			ip_header = (struct ip*) buffer;
			icmp_header = (struct icmp*) (buffer + 4 * ip_header->ip_hl);
			r_ip = &ip_header -> ip_src;
			
			
			if (icmp_header -> icmp_type == ICMP_TIME_EXCEEDED){
				
				ip_header = (struct ip *)((uint8_t *) icmp_header+8);
				icmp_header = (struct icmp *) ((uint8_t *) ip_header + ip_header->ip_hl * 4);
				
				if (seq_start <= icmp_header -> icmp_seq && 
								icmp_header -> icmp_seq  <= seq_start+num_of_packets &&
								icmp_header -> icmp_id==pid){

					char ip_1[30];
					inet_ntop(AF_INET, r_ip, ip_1, sizeof(ip_1));
					printf("%s", ip_1);
					responses++;		
					return responses;
				}			
			}
			else if (icmp_header -> icmp_type == ICMP_ECHOREPLY && 
				seq_start <= icmp_header -> icmp_seq && 
				icmp_header -> icmp_seq  <= seq_start+num_of_packets &&
				icmp_header -> icmp_id==pid){
					char ip_1[30];
					inet_ntop(AF_INET, r_ip, ip_1, sizeof(ip_1));
					printf("%s", ip_1);
					responses++;		
					return responses;	
				}

		}
	}
	return responses;
}
